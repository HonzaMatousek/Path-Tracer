//
// Created by honza on 02.08.20.
//

#include "SphereEnd.h"

SphereEnd::SphereEnd(const Vector3D &center, double radius, const Vector3D &direction, double rimRadius, const Material &material) : Sphere(center - Vector3D(direction).Normalize() * radius, radius, material), direction(direction), rimRadius(rimRadius) {
    this->direction.Normalize();
    double maxSin = rimRadius / radius;
    maxCos = sqrt(1 - maxSin * maxSin);
}

void SphereEnd::Intersect(const Ray &ray, Intersection &intersection) const {
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
        Vector3D v = (point - center).Normalize();
        double cosAlpha = v.Dot(direction);
        if(cosAlpha > maxCos) {
            intersection.ChooseCloser(Intersection(t0, this, point - center));
            return;
        }
    }

    if (t1 >= 0) {
        Vector3D point = ray.Point(t1);
        Vector3D v = (point - center).Normalize();
        double cosAlpha = v.Dot(direction);
        if(cosAlpha > maxCos) {
            intersection.ChooseCloser(Intersection(t1, this, point - center));
        }
    }
}


