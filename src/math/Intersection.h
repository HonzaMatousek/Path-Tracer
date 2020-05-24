#pragma once

#include "Vector3D.h"
#include "Ray.h"
#include "../material/Material.h"

class Intersection {
public:
    Vector3D point;
    Vector3D normal;
    double t;
    Material material;

    Intersection(const Vector3D & point, const Vector3D & normal, double t, const Material & material);

    [[ nodiscard ]]
    Ray Reflect(const Ray & incoming);

    bool operator < (const Intersection & rhs) const;
};
