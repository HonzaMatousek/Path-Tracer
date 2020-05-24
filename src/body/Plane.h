#pragma once

#include "Body.h"

class Plane : public Body {
    Vector3D origin;
    Vector3D normal;
public:
    Plane(const Vector3D & origin, const Vector3D & normal, const Material & material = Material());
    ~Plane() override = default;

    [[ nodiscard ]]
    std::unique_ptr<Intersection> Intersect(const Ray &ray) const override;
};
