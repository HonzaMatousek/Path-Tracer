#pragma once

#include "Vector3D.h"
#include "Ray.h"
#include "../material/Material.h"
#include <random>

class Body;

class Intersection {
public:
    double t;
    const Body * body;
    Vector3D localCoordinates;

    Intersection() : t(std::numeric_limits<double>::infinity()), body(nullptr), localCoordinates(0,0,0) {}
    Intersection(double t, const Body * body, Vector3D&& localCoordinates) : t(t), body(body), localCoordinates(localCoordinates) {}

    [[ nodiscard ]]
    Ray Reflect(const Ray & incoming, double & powerMultiplier, std::mt19937 & generator);

    bool operator < (const Intersection & rhs) const {
        return t < rhs.t;
    }

    void ChooseCloser(Intersection && other) {
        if(other < *this) *this = std::move(other);
    }

    operator bool () const {
        return t < std::numeric_limits<double>::infinity();
    }

    [[ nodiscard ]]
    Material GetMaterial() const;

    [[ nodiscard ]]
    Vector3D GetNormal() const;
};
