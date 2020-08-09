#pragma once

#include "Body.h"

class Plane : public Body {
protected:
    Vector3D origin;
    Vector3D normal;
    Plane(const Vector3D & origin, const Vector3D & normal, const Vector3D & lowerCorner, const Vector3D & upperCorner, const Material & material = Material());
public:
    Plane(const Vector3D & origin, const Vector3D & normal, const Material & material = Material());
    ~Plane() override = default;

    void Intersect(const Ray & ray, Intersection & intersection) const override;
};
