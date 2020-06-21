#pragma once

#include <vector>
#include "Body.h"
#include "KDTree.h"

class Camera;
class Image;
class LineEmployer;

class Scene : public Body {
    std::vector<std::unique_ptr<Body>> bodies;
    std::unique_ptr<Body> kdTree;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Image> image;
    int image_width;
    int image_height;
    int samples;
public:
    Scene();
    Scene(std::string fileName);
    ~Scene() override;

    void AddBody(std::unique_ptr<Body> body);

    void Compile();
    void Intersect(const Ray & ray, Intersection & intersection) const override;

    void Render();
    void RenderCore(std::mt19937& generator, LineEmployer & lineEmployer) const;
};
