#pragma once

#include "Body.h"

class KDTree : public Body {
    std::unique_ptr<KDTree> lower;
    std::unique_ptr<KDTree> upper;
    std::vector<Body*> bodies;
    Vector3D::Axis splitAxis;
    [[nodiscard]] double Price(const std::vector<Body*> & bodies, Vector3D::Axis axis, double splitPosition, size_t lowerCount, size_t upperCount) const;
    void Intersect(const Ray &ray, Intersection &intersection, double t1, double t2) const;
public:
    KDTree(const std::vector<Body*> & bodies, const Vector3D & lowerCorner, const Vector3D & upperCorner);
    void Intersect(const Ray &ray, Intersection &intersection) const override;
};
