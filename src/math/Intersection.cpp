#include "Intersection.h"

Intersection::Intersection(const Vector3D &point, const Vector3D &normal, double t, const Material & material) : point(point), normal(normal), t(t), material(material) {
    this->normal.Normalize();
}

Ray Intersection::Reflect(const Ray &incoming) {
    return Ray(point, incoming.direction - normal * (2 * incoming.direction.Dot(normal)));
}

bool Intersection::operator<(const Intersection &rhs) const {
    return t < rhs.t;
}
