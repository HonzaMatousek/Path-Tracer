#pragma once

#include "Body.h"

class Sphere : public Body {
    Vector3D center;
    double radius;

public:
    Sphere(const Vector3D & center, double radius);
    virtual ~Sphere() = default;

    [[ nodiscard ]]
    std::unique_ptr<Intersection> Intersect(const Ray &ray) const override;
};
