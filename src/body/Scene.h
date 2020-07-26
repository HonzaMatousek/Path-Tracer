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

class Scene : public Body {
    std::vector<std::unique_ptr<Body>> bodies;
    std::unique_ptr<Body> kdTree;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Image> image;
    std::unique_ptr<Effect> effect;
    int image_width;
    int image_height;
    int samples;
    std::map<std::string, TexturedMaterial> materialLibrary;
public:
    Scene();
    Scene(const std::string & fileName);
    ~Scene() override;

    void AddBody(std::unique_ptr<Body> body);

    void Compile();
    void Intersect(const Ray & ray, Intersection & intersection) const override;

    void LoadMTL(const std::string & fileName);
    const TexturedMaterial * GetMaterial(const std::string & materialName) const;

    void Render();
    void RenderCore(std::mt19937& generator, LineEmployer & lineEmployer) const;
};
