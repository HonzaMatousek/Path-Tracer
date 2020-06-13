#pragma once

#include <cmath>
#include <initializer_list>

class Vector3D {
    bool normalized = false;
    //Vector3D(double x, double y, double z, bool n) : normalized(n), x(x), y(y), z(z) {}
public:
    double x;
    double y;
    double z;

    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    [[ nodiscard ]]
    Vector3D   operator +  (const Vector3D & rhs) const {
        return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vector3D & operator += (const Vector3D & rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        normalized = false;
        return *this;
    }

    [[ nodiscard ]]
    Vector3D   operator -  (const Vector3D & rhs) const {
        return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vector3D & operator -= (const Vector3D & rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        normalized = false;
        return *this;
    }

    [[ nodiscard ]]
    bool       operator == (const Vector3D & rhs) const {
        // todo: use epsilon
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    [[ nodiscard ]]
    bool       operator != (const Vector3D & rhs) const {
        return !(*this == rhs);
    }

    [[ nodiscard ]]
    Vector3D   operator *  (double scale) const {
        return Vector3D(x * scale, y * scale, z * scale);
    }

    Vector3D & operator *= (double scale) {
        x *= scale;
        y *= scale;
        z *= scale;
        normalized = scale == 1 ? normalized : false;
        return *this;
    }

    [[ nodiscard ]]
    Vector3D   operator /  (double scale) const {
        return Vector3D(x / scale, y / scale, z / scale);
    }

    Vector3D & operator /= (double scale) {
        x /= scale;
        y /= scale;
        z /= scale;
        normalized = scale == 1 ? normalized : false;
        return *this;
    }

    [[ nodiscard ]]
    Vector3D   Cross       (const Vector3D & rhs) const {
        return Vector3D(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
    }

    [[ nodiscard ]]
    double     Dot         (const Vector3D & rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    [[ nodiscard ]]
    double     Length() const {
        return normalized ? 1 : std::sqrt(Square());
    }

    [[ nodiscard ]]
    double     Square() const {
        return normalized ? 1 : Dot(*this);
    }

    [[ nodiscard ]]
    double     Distance(const Vector3D & rhs) const {
        return (rhs - *this).Length();
    }

    Vector3D & Normalize() {
        if(!normalized) {
            *this /= Length();
            normalized = true;
        }
        return *this;
    }
    
    [[ nodiscard ]]
    Vector3D Min(const Vector3D & other) const {
        return Vector3D(std::min(x, other.x), std::min(y, other.y), std::min(z, other.z));
    }

    [[ nodiscard ]]
    Vector3D Max(const Vector3D & other) const {
        return Vector3D(std::max(x, other.x), std::max(y, other.y), std::max(z, other.z));
    }

    [[ nodiscard ]]
    double AABBSurface() const {
        return x * y + x * z + y * z;
    }

    //static Vector3D Zero();
    //static Vector3D Up();

    using Axis = double Vector3D::*;
    static const std::initializer_list<Axis> axes;
};

inline const std::initializer_list<Vector3D::Axis> Vector3D::axes = {
        &Vector3D::x,
        &Vector3D::y,
        &Vector3D::z,
};
