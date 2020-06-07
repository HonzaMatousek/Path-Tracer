#include "Plane.h"

Plane::Plane(const Vector3D &origin, const Vector3D &normal, const Material &material) : Body(material), origin(origin), normal(normal) {}

void Plane::Intersect(const Ray & ray, Intersection & intersection) const {
    double denominator = normal.Dot(ray.direction);
    if (denominator < -0.00000000001f) // your favorite epsilon
    {
        double t = (origin - ray.origin).Dot(normal) / denominator;
        if (t >= 0) {
            Vector3D point = ray.Point(t);
            intersection.ChooseCloser(Intersection(point, normal, t, material));
        }
    }
}
