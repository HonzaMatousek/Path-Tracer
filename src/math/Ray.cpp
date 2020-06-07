#include "Ray.h"

Ray::Ray(const Vector3D &origin, const Vector3D &direction) : origin(origin), direction(direction) {
    this->direction.Normalize();
}

Vector3D Ray::Point(double t) const {
    return origin + direction * t;
}

bool Ray::IntersectAABB(const Vector3D &l, const Vector3D &u) const {
    Vector3D lo = l - origin;
    Vector3D uo = u - origin;
    
    double txmin = lo.x / direction.x;
    double txmax = uo.x / direction.x;

    if (txmin > txmax) std::swap(txmin, txmax);

    double tymin = lo.y / direction.y;
    double tymax = uo.y / direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((txmin > tymax) || (tymin > txmax))
        return false;

    if (tymin > txmin)
        txmin = tymin;

    if (tymax < txmax)
        txmax = tymax;

    double tzmin = lo.z / direction.z;
    double tzmax = uo.z / direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((txmin > tzmax) || (tzmin > txmax))
        return false;

    if (tzmin > txmin)
        txmin = tzmin;

    if (tzmax < txmax)
        txmax = tzmax;

    return true;
}
