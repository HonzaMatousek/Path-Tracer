#pragma once

#include "Vector3D.h"
#include "Ray.h"

class Intersection {
public:
    Vector3D point;
    Vector3D normal;
    double t;

    Intersection(const Vector3D & point, const Vector3D & normal, double t);

    [[ nodiscard ]]
    Ray Reflect(const Ray & incoming);

    bool operator < (const Intersection & rhs) const;
};
