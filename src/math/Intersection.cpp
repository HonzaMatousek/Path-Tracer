#include "Intersection.h"

Intersection::Intersection(const Vector3D &point, const Vector3D &normal, double t) : point(point), normal(normal), t(t) {}

Ray Intersection::Reflect(const Ray &incoming) {
    return Ray(Vector3D(0,0,0), Vector3D(1,1,1));
}

bool Intersection::operator<(const Intersection &rhs) const {
    return t < rhs.t;
}
