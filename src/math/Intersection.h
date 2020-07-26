#pragma once

#include "Vector3D.h"
#include "Ray.h"
#include "../material/Material.h"
#include "../material/Environment.h"
#include <random>
#include <stack>

class Body;

class Intersection {
public:
    double t;
    const Body * body;
    Vector3D localCoordinates;

    Intersection(const Body * body, const Ray & ray) : t(std::numeric_limits<double>::infinity()), body(body), localCoordinates(ray.direction) {}
    Intersection(double t, const Body * body, Vector3D&& localCoordinates) : t(t), body(body), localCoordinates(localCoordinates) {}

    [[ nodiscard ]]
    Ray Reflect(const Ray & incoming, Color & powerMultiplier, std::stack<Environment> & environments, std::mt19937 & generator);

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
    Vector3D GetNormal(const Vector3D & materialNormal) const;
};
