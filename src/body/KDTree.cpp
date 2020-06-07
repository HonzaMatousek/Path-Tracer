#include "KDTree.h"

void KDTree::Intersect(const Ray &ray, Intersection &intersection) const {

}

KDTree::KDTree(const std::vector<Body*> &bodies, const Vector3D &lowerCorner,
               const Vector3D &upperCorner) : Body(Material(), lowerCorner, upperCorner), bodies(bodies) {
    if(bodies.size() < 2) {
        // no need to construct tree
        this->bodies = bodies;
        return;
    }
    //
    double bestPrice = (upperCorner - lowerCorner).AABBSurface() * bodies.size();
    Vector3D::Axis bestAxis = &Vector3D::x;
    double bestSplit = 0;
    for(auto & body : bodies) {
        auto l = body->lowerCorner;
        auto u = body->upperCorner;
        for(auto axis : Vector3D::axes) {
            double price1 = Price(axis, l.*axis);
            if(price1 < bestPrice) {
                bestAxis = axis;
                bestSplit = l.*axis;
                bestPrice = price1;
            }
            double price2 = Price(axis, u.*axis);
            if(price2 < bestPrice) {
                bestAxis = axis;
                bestSplit = u.*axis;
                bestPrice = price2;
            }
        }
    }
    //
    if(bestPrice == (upperCorner - lowerCorner).AABBSurface() * bodies.size()) {
        // no better split
        this->bodies = bodies;
        return;
    }
    auto lowerCornerSplit = lowerCorner;
    lowerCornerSplit.*bestAxis = bestSplit;
    auto upperCornerSplit = upperCorner;
    upperCornerSplit.*bestAxis = bestSplit;
    std::vector<Body*> lowerBodies;
    std::vector<Body*> upperBodies;
    for(auto & body : bodies) {
        if(body->IntersectAABB(lowerCorner, upperCornerSplit)) {
            lowerBodies.push_back(body);
        }
        if(body->IntersectAABB(lowerCornerSplit, upperCorner)) {
            upperBodies.push_back(body);
        }
    }
    lower = std::make_unique<KDTree>(lowerBodies, lowerCorner, upperCornerSplit);
    upper = std::make_unique<KDTree>(upperBodies, lowerCornerSplit, upperCorner);
}

double KDTree::Price(Vector3D::Axis axis, double splitPosition) const {
    auto lowerCornerSplit = lowerCorner;
    lowerCornerSplit.*axis = splitPosition;
    auto upperCornerSplit = upperCorner;
    upperCornerSplit.*axis = splitPosition;
    size_t lowerCount = 0;
    size_t upperCount = 0;
    for(auto & body : bodies) {
        if(body->IntersectAABB(lowerCorner, upperCornerSplit)) {
            lowerCount++;
        }
        if(body->IntersectAABB(lowerCornerSplit, upperCorner)) {
            upperCount++;
        }
    }
    return lowerCount * (upperCornerSplit - lowerCorner).AABBSurface() + upperCount * (upperCorner - lowerCornerSplit).AABBSurface();
}
