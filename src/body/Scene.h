#pragma once

#include <vector>
#include "Body.h"

class Scene : public Body {
    std::vector<std::unique_ptr<Body>> bodies;
public:
    Scene() = default;
    ~Scene() override = default;

    void AddBody(std::unique_ptr<Body> body);

    void Intersect(const Ray & ray, Intersection & intersection) const override;
};
