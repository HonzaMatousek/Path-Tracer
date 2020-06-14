#pragma once

#include "Body.h"

class Plane : public Body {
    Vector3D origin;
    Vector3D normal;
public:
    Plane(const Vector3D & origin, const Vector3D & normal, const Material & material = Material());
    ~Plane() override = default;

    void Intersect(const Ray & ray, Intersection & intersection) const override;

    [[ nodiscard ]]
    Vector3D GetNormal(const Vector3D & localCoordinates) const override {
        return normal;
    }
};
