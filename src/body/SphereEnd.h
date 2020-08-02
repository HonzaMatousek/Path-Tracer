#pragma once

#include "Sphere.h"

class SphereEnd : public Sphere {
    Vector3D direction;
    double rimRadius;
    double maxCos;
public:
    SphereEnd(const Vector3D & center, double radius, const Vector3D & direction, double rimRadius, const Material & material = Material());
    ~SphereEnd() override = default;

    void Intersect(const Ray &ray, Intersection &intersection) const override;
};
