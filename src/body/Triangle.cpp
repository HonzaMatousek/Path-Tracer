//
// Created by honza on 31.05.20.
//

#include "Triangle.h"

Triangle::Triangle(const Vector3D &a, const Vector3D &b, const Vector3D &c, const Material &material) : Body(std::make_unique<FlatInterpolator<Material>>(material), std::make_unique<PlaneNormalInterpolator>((b - a).Cross(c - a).Normalize()), a.Min(b).Min(c), a.Max(b).Max(c)), a(a), b(b), c(c), u(b - a), v(c - a), normal(u.Cross(v)) {
    this->normal.Normalize();
    dotUU = u.Dot(u);
    dotUV = u.Dot(v);
    dotVV = v.Dot(v);
    invDenom = 1 / ( dotVV * dotUU - dotUV * dotUV);
}

void Triangle::Intersect(const Ray & ray, Intersection & intersection) const {
    double denominator = normal.Dot(ray.direction);
    if (denominator < -0.00000000001f || denominator > 0.00000000001f) // your favorite epsilon
    {
        double t = (a - ray.origin).Dot(normal) / denominator;
        if (t >= 0) {
            Vector3D point = ray.Point(t);
            Vector3D w = point - a;
            double dotVW = v.Dot(w);
            double dotUW = u.Dot(w);
            double _v = (dotUU * dotVW - dotUV * dotUW) * invDenom;
            double _u = (dotVV * dotUW - dotUV * dotVW) * invDenom;
            if(_u < 0 || _v < 0 || _v + _u > 1) return;
            intersection.ChooseCloser(Intersection(t, this, Vector3D(_u, _v, 0)));
        }
    }
}
