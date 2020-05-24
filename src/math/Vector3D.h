#pragma once

class Vector3D {
public:
    double x;
    double y;
    double z;

    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    [[ nodiscard ]]
    Vector3D   operator +  (const Vector3D & rhs) const;
    Vector3D & operator += (const Vector3D & rhs);
    [[ nodiscard ]]
    Vector3D   operator -  (const Vector3D & rhs) const;
    Vector3D & operator -= (const Vector3D & rhs);

    [[ nodiscard ]]
    bool       operator == (const Vector3D & rhs) const;

    [[ nodiscard ]]
    bool       operator != (const Vector3D & rhs) const;

    [[ nodiscard ]]
    Vector3D   operator *  (double scale) const;
    Vector3D & operator *= (double scale);
    [[ nodiscard ]]
    Vector3D   operator /  (double scale) const;
    Vector3D & operator /= (double scale);

    [[ nodiscard ]]
    Vector3D   Cross       (const Vector3D & rhs) const;
    [[ nodiscard ]]
    double     Dot         (const Vector3D & rhs) const;

    [[ nodiscard ]]
    double     Length() const;
    [[ nodiscard ]]
    double     Square() const;
    [[ nodiscard ]]
    double     Distance(const Vector3D & rhs) const;
    Vector3D & Normalize();

    //static Vector3D Zero();
    //static Vector3D Up();
};
