#pragma once

#include "Vector3D.h"
#include "Ray.h"
#include "../material/Material.h"
#include <random>

class Intersection {
public:
    Vector3D point;
    Vector3D normal;
    double t;
    Material material;

    Intersection() : point(0, 0, 0), normal(0, 0, 0), t(std::numeric_limits<double>::infinity()), material() {}
    Intersection(const Vector3D & point, const Vector3D & normal, double t, const Material & material);

    [[ nodiscard ]]
    Ray Reflect(const Ray & incoming, double & powerMultiplier, std::mt19937 & generator);

    bool operator < (const Intersection & rhs) const;

    void ChooseCloser(Intersection && other) {
        if(other < *this) std::swap(other, *this);
    }

    operator bool () const {
        return t < std::numeric_limits<double>::infinity();
    }
};
