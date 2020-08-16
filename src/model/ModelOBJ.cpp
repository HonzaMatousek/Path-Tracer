#include "ModelOBJ.h"
#include "../body/Triangle.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../math/Transform.h"
#include "../command/SceneBuilder.h"
#include "../body/Scene.h"
#include "../command/CommandLoadMaterialLibrary.h"

const Vector3D & findIndex(std::vector<Vector3D> & vertices, int index) {
    if(index > 0) {
        return vertices[index - 1];
    }
    else {
        return vertices[vertices.size() + index];
    }
}

void readFaceVertex(std::istream & is, int & vIndex, int & vtIndex, int & vnIndex) {
    is >> vIndex;
    vtIndex = 0;
    vnIndex = 0;
    if(is.peek() == '/') {
        is.ignore(1);
        if(is.peek() != '/') {
            is >> vtIndex;
        }
        if(is.peek() == '/') {
            is.ignore(1);
            is >> vnIndex;
        }
    }
}

void ModelOBJ::Import(const std::string &fileName, const Transform & transform, SceneBuilder &sceneBuilder) {
    const TexturedMaterial * current_material = sceneBuilder.GetMaterial("whiteDiffuse");
    std::ifstream file(fileName);
    if(!file) {
        std::ostringstream os;
        os << "Model \"" << fileName << "\" not exists.";
        throw std::runtime_error(os.str());
    }
    std::string line;
    std::vector<Vector3D> vertices; // vertex - vertices
    std::vector<Vector3D> verticesTexture; // vertex - vertices
    std::vector<Vector3D> verticesNormal; // vertex - vertices
    double lowest = 10000;
    for(int lineCounter = 1; std::getline(file, line); lineCounter++) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(command == "v") {
            double x, y, z;
            lineStream >> x >> y >> z;
            vertices.emplace_back(transform.Apply(Vector3D(x, y, z)));
            lowest = std::min(lowest, y);
        }
        if(command == "vt") {
            double x, y, z;
            lineStream >> x >> y >> z;
            verticesTexture.emplace_back(Vector3D(x, y, z));
        }
        if(command == "vn") {
            double x, y, z;
            lineStream >> x >> y >> z;
            verticesNormal.emplace_back(transform.ApplyWithoutTranslation(Vector3D(x, y, z)));
        }
        else if(command == "f") {
            int a, b, c, at, bt, ct, an, bn, cn;
            readFaceVertex(lineStream, a, at, an);
            readFaceVertex(lineStream, b, bt, bn);
            readFaceVertex(lineStream, c, ct, cn);
            auto triangle = std::make_unique<Triangle>(findIndex(vertices, a), findIndex(vertices, b), findIndex(vertices, c), current_material->base);
            if(at && bt && ct) {
                triangle->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                        std::make_unique<TriangleInterpolator<Vector3D>>(
                                findIndex(verticesTexture, at), findIndex(verticesTexture, bt), findIndex(verticesTexture, ct)
                        ),
                        current_material
                ));
                auto edge1 = findIndex(vertices, b) - findIndex(vertices, a);
                auto edge2 = findIndex(vertices, c) - findIndex(vertices, a);
                auto deltaUV1 = findIndex(verticesTexture, bt) - findIndex(verticesTexture, at);
                auto deltaUV2 = findIndex(verticesTexture, ct) - findIndex(verticesTexture, at);
                double f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                Vector3D tangent;
                tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

                Vector3D bitangent;
                bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

                triangle->SetNormalInterpolator(std::make_unique<PlaneNormalInterpolator>(
                        tangent.Normalize(), bitangent.Normalize(), (findIndex(vertices, b) - findIndex(vertices, a)).Cross(findIndex(vertices, c) - findIndex(vertices, a)).Normalize()
                ));
            }
            if(an && bn && cn) {
                std::unique_ptr<Interpolator<Transform>> originalNormalInterpolator;
                triangle->SwapNormalInterpolator(originalNormalInterpolator);
                triangle->SetNormalInterpolator(std::make_unique<PlaneNormalRoundingInterpolator>(
                        std::move(originalNormalInterpolator),
                        findIndex(verticesNormal, an), findIndex(verticesNormal, bn), findIndex(verticesNormal, cn)
                ));
            }
            sceneBuilder.scene.AddBody(std::move(triangle));
        }
        else if(command == "mtllib") {
            CommandLoadMaterialLibrary cmd;
            cmd.Execute(sceneBuilder, lineStream);
        }
        else if(command == "usemtl") {
            std::string materialName;
            lineStream >> materialName;
            current_material = sceneBuilder.GetMaterial(materialName);
        }
    }
}
