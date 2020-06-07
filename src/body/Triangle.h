#pragma once

#include "../math/Vector3D.h"
#include "Body.h"

class Triangle : public Body {
    Vector3D a, b, c;
    Vector3D u, v;
    Vector3D normal;
public:
    Triangle(const Vector3D & a, const Vector3D & b, const Vector3D & c, const Material &material);

    void Intersect(const Ray & ray, Intersection & intersection) const override;
};
