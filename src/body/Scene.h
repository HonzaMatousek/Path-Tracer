#pragma once

#include <vector>
#include <map>
#include "Body.h"
#include "KDTree.h"
#include "../material/TexturedMaterial.h"

class Camera;
class Image;
class Effect;
class LineEmployer;
class Command;
class SceneBuilder;

class Scene : public Body {
    std::vector<std::unique_ptr<Body>> bodies;
    std::unique_ptr<Body> kdTree;
    std::map<std::string, TexturedMaterial> materialLibrary;
    std::map<std::string, std::unique_ptr<Command>> commands;
public:
    Scene();
    Scene(const std::string & fileName);
    ~Scene() override;

    void AddBody(std::unique_ptr<Body> body);

    void Compile();
    void Intersect(const Ray & ray, Intersection & intersection) const override;

    friend SceneBuilder;
};
