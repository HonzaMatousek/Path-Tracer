#pragma once

#include "../math/Vector3D.h"
#include "Sphere.h"

class Cylinder : public Sphere {
    Vector3D center;
    Vector3D direction;
    double radius;
    double radiusSquared;
    double lowCap;
    double highCap;
    Transform circleBasis;
    Transform worldBasis;
public:
    Cylinder(const Vector3D & center, const Vector3D & direction, double radius, double lowCap, double highCap, const Material & material);
    ~Cylinder() override = default;

    void Intersect(const Ray &ray, Intersection &intersection) const override;
};
