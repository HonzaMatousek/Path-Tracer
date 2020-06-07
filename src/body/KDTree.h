#pragma once

#include "Body.h"

class KDTree : public Body {
    std::unique_ptr<KDTree> lower;
    std::unique_ptr<KDTree> upper;
    std::vector<Body*> bodies;
    [[nodiscard]] double Price(const std::vector<Body*> & bodies, Vector3D::Axis axis, double splitPosition) const;
public:
    KDTree(const std::vector<Body*> & bodies, const Vector3D & lowerCorner, const Vector3D & upperCorner);
    void Intersect(const Ray &ray, Intersection &intersection) const override;
};
