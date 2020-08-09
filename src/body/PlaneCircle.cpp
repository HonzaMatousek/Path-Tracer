//
// Created by honza on 09.08.20.
//

#include "PlaneCircle.h"

PlaneCircle::PlaneCircle(const Vector3D &origin, const Vector3D &normal, double radius, const Material &material) : Plane(origin, normal, origin - Vector3D(1, 1, 1) * radius, origin + Vector3D(1, 1, 1) * radius, material), radius(radius) {}

void PlaneCircle::Intersect(const Ray &ray, Intersection &intersection) const {
    double denominator = normal.Dot(ray.direction);
    if (denominator < -0.00000000001f) // your favorite epsilon
    {
        double t = (origin - ray.origin).Dot(normal) / denominator;
        if (t >= 0) {
            Vector3D point = ray.Point(t);
            if ((point - origin).Length() < radius) {
                intersection.ChooseCloser(Intersection(t, this, Vector3D(0, 0, 0)));
            }
        }
    }
}
