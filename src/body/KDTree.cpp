#include <map>
#include "KDTree.h"

struct SplittingInfo {
    size_t lowerChange = 0;
    size_t upperChange = 0;
};

void KDTree::Intersect(const Ray &ray, Intersection &intersection) const {
    double tl, tu;
    if(ray.IntersectAABB(lowerCorner, upperCorner, tl, tu)) {
        Intersect(ray, intersection, tl, tu);
    }
}

KDTree::KDTree(const std::vector<Body*> &bodies, const Vector3D &lowerCorner,
               const Vector3D &upperCorner) : Body(nullptr, nullptr, lowerCorner, upperCorner) {
    if(bodies.size() < 2) {
        // no need to construct tree
        this->bodies = bodies;
        return;
    }
    //
    double bestPrice = (upperCorner - lowerCorner).AABBSurface() * bodies.size();
    Vector3D::Axis bestAxis = &Vector3D::x;
    double bestSplit = 0;
    constexpr double eps = 0.00001;
    for(auto axis : Vector3D::axes) {
        std::map<double, SplittingInfo> splittingInfos;
        for (auto &body : bodies) {
            splittingInfos[body->lowerCorner.*axis - eps].lowerChange += 1;
            splittingInfos[body->upperCorner.*axis + eps].upperChange += 1;
        }
        size_t lowerCount = 0;
        size_t upperCount = bodies.size();
        for (auto & [ split, splittingInfo ] : splittingInfos) {
            upperCount -= splittingInfo.upperChange;
            double price = Price(bodies, axis, split, lowerCount, upperCount);
            if(price < bestPrice) {
                bestAxis = axis;
                bestSplit = split;
                bestPrice = price;
            }
            lowerCount += splittingInfo.lowerChange;
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
    splitAxis = bestAxis;
    lower = std::make_unique<KDTree>(lowerBodies, lowerCorner, upperCornerSplit);
    upper = std::make_unique<KDTree>(upperBodies, lowerCornerSplit, upperCorner);
}

double KDTree::Price(const std::vector<Body*> & bodies, Vector3D::Axis axis, double splitPosition, size_t lowerCount, size_t upperCount) const {
    auto lowerCornerSplit = lowerCorner;
    lowerCornerSplit.*axis = splitPosition;
    auto upperCornerSplit = upperCorner;
    upperCornerSplit.*axis = splitPosition;
    return lowerCount * (upperCornerSplit - lowerCorner).AABBSurface() + upperCount * (upperCorner - lowerCornerSplit).AABBSurface();
}

void KDTree::Intersect(const Ray &ray, Intersection &intersection, double t1, double t2) const {
    if(!lower) {
        for(auto body : bodies) {
            body->Intersect(ray, intersection);
        }
        return;
    }
    double ts = (lower->upperCorner.*splitAxis - ray.origin.*splitAxis) / ray.direction.*splitAxis;
    if(ts > t1) {
        (ray.direction.*splitAxis > 0 ? lower : upper)->Intersect(ray, intersection, t1, std::min(ts, t2));
    }
    if(intersection.t <= ts) return; // do not visit further node if we have closer intersection
    if(ts < t2) {
        (ray.direction.*splitAxis > 0 ? upper : lower)->Intersect(ray, intersection, std::max(ts, t1), t2);
    }
}
