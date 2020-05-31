//
// Created by honza on 31.05.20.
//

#include "Triangle.h"

Triangle::Triangle(const Vector3D &a, const Vector3D &b, const Vector3D &c, const Material &material) : Body(material), a(a), b(b), c(c), u(b - a), v(c - a), normal(u.Cross(v)) {}

std::unique_ptr<Intersection> Triangle::Intersect(const Ray &ray) const {
    double denominator = normal.Dot(ray.direction);
    if (denominator < -0.00000000001f) // your favorite epsilon
    {
        double t = (a - ray.origin).Dot(normal) / denominator;
        if (t >= 0) {
            // I am in the plane of the triangle
            Vector3D point = ray.Point(t);
            Vector3D point_diff = point - a;
            // a + _u * u + _v * v = point
            double _u, _v;
            if(std::abs(u.y * v.z - u.z * v.y) < 0.0001) {
                _u = (point_diff.x * v.y - point_diff.y * v.x) / (u.x * v.y - u.y * v.x);
                _v = (u.x * point_diff.y - u.y * point_diff.x) / (u.x * v.y - u.y * v.x);
            }
            else {
                _u = (point_diff.y * v.z - point_diff.z * v.y) / (u.y * v.z - u.z * v.y);
                _v = (u.y * point_diff.z - u.z * point_diff.y) / (u.y * v.z - u.z * v.y);
            }
            if(_u < 0 || _v < 0 || _v + _u > 1) return nullptr;
            return std::make_unique<Intersection>(point, normal, t, material);
        }
    }
    return nullptr;
}
