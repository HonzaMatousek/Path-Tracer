#pragma once

#include "Vector3D.h"

class Ray {
public:
    Vector3D origin;
    Vector3D direction;

    Ray(const Vector3D & origin, const Vector3D & direction) : origin(origin), direction(direction) {
        this->direction.Normalize();
    }

    [[ nodiscard ]]
    Vector3D Point(double t) const {
        return origin + direction * t;
    }

    [[ nodiscard ]]
    bool IntersectAABB(const Vector3D & l, const Vector3D & u) const {
        double tl = -std::numeric_limits<double>::infinity();
        double tu = std::numeric_limits<double>::infinity();

        for (auto axis : Vector3D::axes) {
            double t1 = (l.*axis - origin.*axis) / direction.*axis;
            double t2 = (u.*axis - origin.*axis) / direction.*axis;
            if (t1 < t2) {
                tl = std::max(tl, t1);
                tu = std::min(tu, t2);
            } else {
                tl = std::max(tl, t2);
                tu = std::min(tu, t1);
            }
            if (tu < 0 || tl > tu) return false;
        }

        return true;
    }

};
