#include <cmath>
#include "Vector3D.h"

Vector3D Vector3D::operator+(const Vector3D & rhs) const {
    return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3D & Vector3D::operator+=(const Vector3D & rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector3D Vector3D::operator-(const Vector3D & rhs) const {
    return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3D & Vector3D::operator-=(const Vector3D & rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

bool Vector3D::operator==(const Vector3D & rhs) const {
    // todo: use epsilon
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Vector3D::operator!=(const Vector3D & rhs) const {
    return !(*this == rhs);
}

Vector3D Vector3D::operator*(double scale) const {
    return Vector3D(x * scale, y * scale, z * scale);
}

Vector3D &Vector3D::operator*=(double scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

Vector3D Vector3D::operator/(double scale) const {
    return Vector3D(x / scale, y / scale, z / scale);
}

Vector3D &Vector3D::operator/=(double scale) {
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
}

Vector3D Vector3D::Cross(const Vector3D &rhs) const {
    return Vector3D(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

double Vector3D::Dot(const Vector3D &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

double Vector3D::Length() const {
    return std::sqrt(Square());
}

double Vector3D::Square() const {
    return Dot(*this);
}

double Vector3D::Distance(const Vector3D &rhs) const {
    return (rhs - *this).Length();
}

Vector3D &Vector3D::Normalize() {
    return *this /= Length();
}
