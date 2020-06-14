#pragma once

#include <memory>
#include "../math/Vector3D.h"
#include "../material/Material.h"

template <typename T>
class Interpolator {
public:
    [[ nodiscard ]]
    virtual T Interpolate(const Vector3D & coordinates) const = 0;
};

template <typename T>
class TriangleInterpolator : public Interpolator<T> {
    T a;
    T ab;
    T ac;
public:
    TriangleInterpolator(const T & a, const T & b, const T & c) : a(a), ab(b - a), ac(c - a) {}

    [[ nodiscard ]]
    T Interpolate(const Vector3D & coordinates) const override {
        return a + ab * coordinates.x + ac * coordinates.y;
    }
};

template <typename T>
class FlatInterpolator : public Interpolator<T> {
    T value;
public:
    FlatInterpolator(const T & value) : value(value) {}

    [[ nodiscard ]]
    T Interpolate(const Vector3D & coordinates) const override {
        return value;
    }
};

class NormalizeInterpolator : public Interpolator<Vector3D> {
public:
    [[ nodiscard ]]
    Vector3D Interpolate(const Vector3D & coordinates) const override {
        return Vector3D(coordinates).Normalize();
    }
};

class NormalDebugInterpolator : public Interpolator<Material> {
    std::unique_ptr<Interpolator<Vector3D>> base;
public:
    NormalDebugInterpolator(std::unique_ptr<Interpolator<Vector3D>> && base) : base(std::move(base)) {}

    [[ nodiscard ]]
    Material Interpolate(const Vector3D & coordinates) const override {
        Vector3D normal(base->Interpolate(coordinates));
        return Material(Color(
            normal.x / 2 + 0.5,
            normal.y / 2 + 0.5,
            normal.z / 2 + 0.5
        ), Color(), false);
    }
};
