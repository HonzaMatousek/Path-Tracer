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

void readFaceVertex(std::istream & is, int & vIndex) {
    is >> vIndex;
    if(is.peek() == '/') {
        std::string waste;
        is >> waste;
    }
}

void ModelOBJ::Import(const std::string &fileName, Scene &scene) {
    //Material beigeDiffuse(Color(0,0,0), Color(0.95,0.95,0.7), false);
    Material beigeDiffuse(Color(0,0,0), Color(0.69,0.065,0.065), false);
    //Material chromium(Color(0, 0, 0), Color(0.5529,0.5529,0.5529), true); // chromium
    Material chromium(Color(0, 0, 0), Color(1,0.71,0.29), true); // gold
    //Material chromium(Color(0,0,0), Color(0.95,0.95,0.7), false);
    //Material chromium(Color(0,0,0), Color(0.95,0.95,0.7), false);
    //Material chromium(Color(0,0,0), Color(0.95,0.015,0.005), false);
    std::string current_mat = "";
    std::ifstream file(fileName);
    std::string line;
    std::vector<Vector3D> vertices; // vertex - vertices
    double lowest = 10000;
    while(std::getline(file, line)) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(command == "v") {
            double x, y, z;
            lineStream >> x >> y >> z;
            vertices.emplace_back(Vector3D(x, y, z));
            lowest = std::min(lowest, y);
        }
        else if(command == "f") {
            int a, b, c;
            readFaceVertex(lineStream, a);
            readFaceVertex(lineStream, b);
            readFaceVertex(lineStream, c);
            auto triangle = std::make_unique<Triangle>(findIndex(vertices, a), findIndex(vertices, b), findIndex(vertices, c), current_mat == "mat_pi4c75f579" ? beigeDiffuse : chromium);
            scene.AddBody(std::move(triangle));
            //scene.AddBody(std::make_unique<Triangle>(findIndex(vertices, a), findIndex(vertices, b), findIndex(vertices, c), beigeDiffuse));
        }
        else if(command == "usemtl") {
            lineStream >> current_mat;
        }
    }
    std::cout << "lowest:" << lowest << std::endl;
}
