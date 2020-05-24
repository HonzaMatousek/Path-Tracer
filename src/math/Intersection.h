#pragma once

#include "Vector3D.h"
#include "Ray.h"

class Intersection {
public:
    Vector3D point;
    Vector3D normal;

    Intersection(const Vector3D & point, const Vector3D & normal);

    [[ nodiscard ]]
    Ray Reflect(const Ray & incoming);
};
