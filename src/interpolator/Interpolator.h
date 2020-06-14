#pragma once

#include "../math/Vector3D.h"

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
