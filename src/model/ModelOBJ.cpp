#include "ModelOBJ.h"
#include "../body/Triangle.h"

#include <iostream>
#include <fstream>
#include <sstream>

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

void ModelOBJ::Import(const std::string &fileName, const Transform & transform, Scene &scene) {
    const TexturedMaterial * current_material = scene.GetMaterial("whiteDiffuse");
    std::ifstream file(fileName);
    std::string line;
    std::vector<Vector3D> vertices; // vertex - vertices
    std::vector<Vector3D> verticesTexture; // vertex - vertices
    std::vector<Vector3D> verticesNormal; // vertex - vertices
    double lowest = 10000;
    while(std::getline(file, line)) {
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
            if(an && bn && cn) {
                triangle->SetNormalInterpolator(std::make_unique<TriangleInterpolator<Vector3D>>(
                        findIndex(verticesNormal, an), findIndex(verticesNormal, bn), findIndex(verticesNormal, cn)
                ));
            }
            if(at && bt && ct) {
                triangle->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                        std::make_unique<TriangleInterpolator<Vector3D>>(
                                findIndex(verticesTexture, at), findIndex(verticesTexture, bt), findIndex(verticesTexture, ct)
                        ),
                        current_material
                ));
            }
            scene.AddBody(std::move(triangle));
        }
        else if(command == "mtllib") {
            std::string mtlFileName;
            lineStream >> mtlFileName;
            scene.LoadMTL(mtlFileName);
        }
        else if(command == "usemtl") {
            std::string materialName;
            lineStream >> materialName;
            current_material = scene.GetMaterial(materialName);
        }
    }
    std::cout << "lowest:" << lowest << std::endl;
}
