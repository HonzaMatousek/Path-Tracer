#pragma once

#include <vector>
#include "Body.h"
#include "KDTree.h"

class Scene : public Body {
    std::vector<std::unique_ptr<Body>> bodies;
    std::unique_ptr<Body> kdTree;
public:
    Scene();
    ~Scene() override = default;

    void AddBody(std::unique_ptr<Body> body);

    void Compile();
    void Intersect(const Ray & ray, Intersection & intersection) const override;
};
