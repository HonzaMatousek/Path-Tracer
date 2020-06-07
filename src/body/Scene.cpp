//
// Created by honza on 24.05.20.
//

#include "Scene.h"

void Scene::AddBody(std::unique_ptr<Body> body) {
    bodies.emplace_back(std::move(body));
}

void Scene::Intersect(const Ray & ray, Intersection & intersection) const {
    if(kdTree) {
        kdTree->Intersect(ray, intersection);
    }
    else for(const auto & body : bodies) {
        body->Intersect(ray, intersection);
    }
}

void Scene::Compile() {
    std::vector<Body*> treeBodies;
    Vector3D l(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    Vector3D u(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
    for(const auto & body : bodies) {
        treeBodies.push_back(body.get());
        l = l.Min(body->lowerCorner);
        u = u.Max(body->upperCorner);
    }
    kdTree = std::make_unique<KDTree>(treeBodies, l, u);
}
