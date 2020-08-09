#pragma once

#include "Plane.h"

class PlaneCircle : public Plane {
    double radius;
public:
    PlaneCircle(const Vector3D & origin, const Vector3D & normal, double radius, const Material & material = Material());
    ~PlaneCircle() override = default;

    void Intersect(const Ray & ray, Intersection & intersection) const override;

};
