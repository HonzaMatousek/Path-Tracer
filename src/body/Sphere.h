#pragma once

#include "Body.h"

class Sphere : public Body {
    Vector3D center;
    double radius;

public:
    Sphere(const Vector3D & center, double radius, const Material & material = Material());
    ~Sphere() override = default;

    void Intersect(const Ray & ray, Intersection & intersection) const override;
};
