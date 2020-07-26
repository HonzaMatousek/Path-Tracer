#include <cmath>
#include "Sphere.h"

Sphere::Sphere(const Vector3D &center, double radius, const Material & material) : Body(std::make_unique<FlatInterpolator<Material>>(material), std::make_unique<NormalizeInterpolator>(radius), center - Vector3D(1,1,1) * std::abs(radius), center + Vector3D(1,1,1) * std::abs(radius)), center(center), radius(radius), radiusSquared(radius * radius) {}

void Sphere::Intersect(const Ray & ray, Intersection & intersection) const {
    // (x-cx)^2 + (y-cy)^2 + (z-cz)^2 = r^2
    // (x,y,z)

    // |(origin + t * direction) - center| = r
    // (origin.x + t * direction.x - center)^2 + (origin.y + t * direction.y - center)^2 ... = r^2

    Vector3D L = center - ray.origin;
    double tca = L.Dot(ray.direction);
    // if (tca < 0) return false;
    double d2 = L.Dot(L) - tca * tca;
    if (d2 > radiusSquared) return;
    double thc = std::sqrt(radiusSquared - d2);
    double t0 = tca - thc;
    double t1 = tca + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 >= 0) {
        Vector3D point = ray.Point(t0);
        intersection.ChooseCloser(Intersection(t0, this, point - center));
        return;
    }

    if (t1 >= 0) {
        Vector3D point = ray.Point(t1);
        intersection.ChooseCloser(Intersection(t1, this, point - center));
    }
}
