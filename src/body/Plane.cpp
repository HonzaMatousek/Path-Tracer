#include "Plane.h"

Plane::Plane(const Vector3D &origin, const Vector3D &normal, const Vector3D &lowerCorner, const Vector3D &upperCorner, const Material &material) : Body(std::make_unique<FlatInterpolator<Material>>(material), std::make_unique<PlaneNormalInterpolator>(normal), lowerCorner, upperCorner), origin(origin), normal(normal) {

}

Plane::Plane(const Vector3D &origin, const Vector3D &normal, const Material &material) : Plane(origin, normal, Vector3D(), Vector3D(), material) {}

void Plane::Intersect(const Ray & ray, Intersection & intersection) const {
    double denominator = normal.Dot(ray.direction);
    if (denominator < -0.00000000001f) // your favorite epsilon
    {
        double t = (origin - ray.origin).Dot(normal) / denominator;
        if (t >= 0) {
            Vector3D point = ray.Point(t);
            intersection.ChooseCloser(Intersection(t, this, Vector3D(0, 0, 0)));
        }
    }
}
