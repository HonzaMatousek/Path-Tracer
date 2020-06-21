#pragma once

#include <array>
#include <cmath>
#include "Vector3D.h"

class Transform {
    double matrix[4][4];
public:
    Transform(double m00, double m01, double m02, double m03, double m10, double m11, double m12, double m13, double m20, double m21, double m22, double m23, double m30, double m31, double m32, double m33) {
        matrix[0][0] = m00;
        matrix[0][1] = m01;
        matrix[0][2] = m02;
        matrix[0][3] = m03;
        matrix[1][0] = m10;
        matrix[1][1] = m11;
        matrix[1][2] = m12;
        matrix[1][3] = m13;
        matrix[2][0] = m20;
        matrix[2][1] = m21;
        matrix[2][2] = m22;
        matrix[2][3] = m23;
        matrix[3][0] = m30;
        matrix[3][1] = m31;
        matrix[3][2] = m32;
        matrix[3][3] = m33;
    }

    static Transform Translation(double x, double y, double z) {
        return Transform(1.0, 0.0, 0.0, x, 0.0, 1.0, 0.0, y, 0.0, 0.0, 1.0, z, 0.0, 0.0, 0.0, 1.0);
    }

    static Transform Scale(double s) {
        return Transform(s, 0.0, 0.0, 0.0, 0.0, s, 0.0, 0.0, 0.0, 0.0, s, 0.0, 0.0, 0.0, 0.0, 1.0);
    }

    static Transform Scale(double x, double y, double z) {
        return Transform(x, 0.0, 0.0, 0.0, 0.0, y, 0.0, 0.0, 0.0, 0.0, z, 0.0, 0.0, 0.0, 0.0, 1.0);
    }

    static Transform RotateY(double angle) {
        return Transform(cos(angle), 0.0, -sin(angle), 0.0, 0.0, 1.0, 0.0, 0.0, sin(angle), 0.0, cos(angle), 0.0, 0.0, 0.0, 0.0, 1.0);
    }

    Transform operator*(const Transform rhs) const {
        return Transform(
            matrix[0][0] * rhs.matrix[0][0] + matrix[0][1] * rhs.matrix[1][0] + matrix[0][2] * rhs.matrix[2][0] + matrix[0][3] * rhs.matrix[3][0],
            matrix[0][0] * rhs.matrix[0][1] + matrix[0][1] * rhs.matrix[1][1] + matrix[0][2] * rhs.matrix[2][1] + matrix[0][3] * rhs.matrix[3][1],
            matrix[0][0] * rhs.matrix[0][2] + matrix[0][1] * rhs.matrix[1][2] + matrix[0][2] * rhs.matrix[2][2] + matrix[0][3] * rhs.matrix[3][2],
            matrix[0][0] * rhs.matrix[0][3] + matrix[0][1] * rhs.matrix[1][3] + matrix[0][2] * rhs.matrix[2][3] + matrix[0][3] * rhs.matrix[3][3],
            matrix[1][0] * rhs.matrix[0][0] + matrix[1][1] * rhs.matrix[1][0] + matrix[1][2] * rhs.matrix[2][0] + matrix[1][3] * rhs.matrix[3][0],
            matrix[1][0] * rhs.matrix[0][1] + matrix[1][1] * rhs.matrix[1][1] + matrix[1][2] * rhs.matrix[2][1] + matrix[1][3] * rhs.matrix[3][1],
            matrix[1][0] * rhs.matrix[0][2] + matrix[1][1] * rhs.matrix[1][2] + matrix[1][2] * rhs.matrix[2][2] + matrix[1][3] * rhs.matrix[3][2],
            matrix[1][0] * rhs.matrix[0][3] + matrix[1][1] * rhs.matrix[1][3] + matrix[1][2] * rhs.matrix[2][3] + matrix[1][3] * rhs.matrix[3][3],
            matrix[2][0] * rhs.matrix[0][0] + matrix[2][1] * rhs.matrix[1][0] + matrix[2][2] * rhs.matrix[2][0] + matrix[2][3] * rhs.matrix[3][0],
            matrix[2][0] * rhs.matrix[0][1] + matrix[2][1] * rhs.matrix[1][1] + matrix[2][2] * rhs.matrix[2][1] + matrix[2][3] * rhs.matrix[3][1],
            matrix[2][0] * rhs.matrix[0][2] + matrix[2][1] * rhs.matrix[1][2] + matrix[2][2] * rhs.matrix[2][2] + matrix[2][3] * rhs.matrix[3][2],
            matrix[2][0] * rhs.matrix[0][3] + matrix[2][1] * rhs.matrix[1][3] + matrix[2][2] * rhs.matrix[2][3] + matrix[2][3] * rhs.matrix[3][3],
            matrix[3][0] * rhs.matrix[0][0] + matrix[3][1] * rhs.matrix[1][0] + matrix[3][2] * rhs.matrix[2][0] + matrix[3][3] * rhs.matrix[3][0],
            matrix[3][0] * rhs.matrix[0][1] + matrix[3][1] * rhs.matrix[1][1] + matrix[3][2] * rhs.matrix[2][1] + matrix[3][3] * rhs.matrix[3][1],
            matrix[3][0] * rhs.matrix[0][2] + matrix[3][1] * rhs.matrix[1][2] + matrix[3][2] * rhs.matrix[2][2] + matrix[3][3] * rhs.matrix[3][2],
            matrix[3][0] * rhs.matrix[0][3] + matrix[3][1] * rhs.matrix[1][3] + matrix[3][2] * rhs.matrix[2][3] + matrix[3][3] * rhs.matrix[3][3]
        );
    }

    [[ nodiscard ]]
    Vector3D Apply(const Vector3D & rhs) const {
        return Vector3D(
                matrix[0][0] * rhs.x + matrix[0][1] * rhs.y + matrix[0][2] * rhs.z + matrix[0][3],
                matrix[1][0] * rhs.x + matrix[1][1] * rhs.y + matrix[1][2] * rhs.z + matrix[1][3],
                matrix[2][0] * rhs.x + matrix[2][1] * rhs.y + matrix[2][2] * rhs.z + matrix[2][3]
        );
    }

    [[ nodiscard ]]
    Vector3D ApplyWithoutTranslation(const Vector3D & rhs) const {
        return Vector3D(
                matrix[0][0] * rhs.x + matrix[0][1] * rhs.y + matrix[0][2] * rhs.z,
                matrix[1][0] * rhs.x + matrix[1][1] * rhs.y + matrix[1][2] * rhs.z,
                matrix[2][0] * rhs.x + matrix[2][1] * rhs.y + matrix[2][2] * rhs.z
        );
    }
};
