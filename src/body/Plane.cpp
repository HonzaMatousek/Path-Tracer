#include "Plane.h"

Plane::Plane(const Vector3D &origin, const Vector3D &normal, const Material &material) : Body(material), origin(origin), normal(normal) {}

std::unique_ptr<Intersection> Plane::Intersect(const Ray &ray) const {
    double denominator = normal.Dot(ray.direction);
    if (denominator < -0.00000000001f) // your favorite epsilon
    {
        double t = (origin - ray.origin).Dot(normal) / denominator;
        if (t >= 0) {
            Vector3D point = ray.Point(t);
            return std::make_unique<Intersection>(point, normal, t, material);
        }
    }
    return nullptr;
}
