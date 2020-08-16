//
// Created by honza on 24.05.20.
//

#include "Scene.h"
#include "../command/Comment.h"
#include "../command/CommandMove.h"
#include "../command/CommandRotateY.h"
#include "../command/CommandPop.h"
#include "../command/CommandLoadModel.h"
#include "../command/CommandSphere.h"
#include "../command/CommandTriangle.h"
#include "../command/CommandLens.h"
#include "../command/CommandLoadMaterialLibrary.h"
#include "../command/CommandSetMaterial.h"
#include "../command/CommandSetBackground.h"
#include "../command/CommandCameraPerspective.h"
#include "../command/CommandCameraOrthogonal.h"
#include "../command/CommandCameraCubeMap.h"
#include "../command/CommandCameraSpherical.h"
#include "../command/CommandEnvironment.h"
#include "../command/CommandEffectDOF.h"
#include "../command/CommandRenderImage.h"

#include <fstream>
#include <sstream>

void Scene::AddBody(std::unique_ptr<Body> body) {
    bodies.emplace_back(std::move(body));
}

void Scene::Intersect(const Ray & ray, Intersection & intersection) const {
    if(kdTree) {
        kdTree->Intersect(ray, intersection);
    }
    else for(const auto & body : bodies) {
        body->Intersect(ray, intersection);
    }
}

void Scene::Compile() {
    std::vector<Body*> treeBodies;
    Vector3D l(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    Vector3D u(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
    for(const auto & body : bodies) {
        treeBodies.push_back(body.get());
        l = l.Min(body->lowerCorner);
        u = u.Max(body->upperCorner);
    }
    kdTree = std::make_unique<KDTree>(treeBodies, l, u);
}

Scene::Scene() : Body(std::make_unique<FlatInterpolator<Material>>(Material())) {
    commands.emplace("#", std::make_unique<Comment>());
    commands.emplace("move", std::make_unique<CommandMove>());
    commands.emplace("rotate_y", std::make_unique<CommandRotateY>());
    commands.emplace("pop_transform", std::make_unique<CommandPop>());
    commands.emplace("model", std::make_unique<CommandLoadModel>());
    commands.emplace("sphere", std::make_unique<CommandSphere>());
    commands.emplace("triangle", std::make_unique<CommandTriangle>());
    commands.emplace("lens", std::make_unique<CommandLens>());
    commands.emplace("current_material", std::make_unique<CommandSetMaterial>());
    commands.emplace("load_material_library", std::make_unique<CommandLoadMaterialLibrary>());
    commands.emplace("background_texture", std::make_unique<CommandSetBackground>());
    commands.emplace("camera", std::make_unique<CommandCameraPerspective>());
    commands.emplace("camera_ortho", std::make_unique<CommandCameraOrthogonal>());
    commands.emplace("camera_cubemap", std::make_unique<CommandCameraCubeMap>());
    commands.emplace("camera_sphere", std::make_unique<CommandCameraSpherical>());
    commands.emplace("camera_env", std::make_unique<CommandEnvironment>());
    commands.emplace("dof", std::make_unique<CommandEffectDOF>());
    commands.emplace("output", std::make_unique<CommandRenderImage>());

}

Scene::Scene(const std::string & fileName) : Scene() {
    //SetMaterialInterpolator(std::make_unique<FlatInterpolator<Material>>(Material(Color(0,0,0), Color(), false)));
    materialLibrary["whiteDiffuse"].base = Material(Color(0,0,0), Color(0.95,0.95,0.95), false);
    materialLibrary["whiteLight"].base = Material(Color(100,100,100), Color(0,0,0));
    materialLibrary["whiteRough"].base = Material(Color(0,0,0), Color(0.95,0.92,0.90), true, 0.0002);
    TexturedMaterial * current_material = &materialLibrary["whiteDiffuse"];
    std::ifstream file(fileName);
    std::string line;
    SceneBuilder sceneBuilder(*this);
    for(int lineCounter = 1; std::getline(file, line); lineCounter++) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(lineStream) {
            Command * cmd;
            try {
                cmd = commands.at(command).get();
            }
            catch(std::exception & e) {
                std::ostringstream os;
                os << "Error (" << fileName << ":" << lineCounter << "): Command \"" << command << "\" not exists.";
                throw std::runtime_error(os.str());
            }
            try {
                cmd->Execute(sceneBuilder, lineStream);
            }
            catch(std::exception & e) {
                std::ostringstream os;
                os << "Error (" << fileName << ":" << lineCounter << "): Command \"" << command << "\" caused error: " << e.what();
                throw std::runtime_error(os.str());
            }
        }
    }
}

Scene::~Scene() = default;
