//
// Created by honza on 24.05.20.
//

#include "Scene.h"

void Scene::AddBody(std::unique_ptr<Body> body) {
    bodies.emplace_back(std::move(body));
}

std::unique_ptr<Intersection> Scene::Intersect(const Ray &ray) const {
    std::unique_ptr<Intersection> closestIntersection = nullptr;
    for(const auto & body : bodies) {
        std::unique_ptr<Intersection> newIntersection = body->Intersect(ray);
        if(!newIntersection) continue;
        if(!closestIntersection) {
            std::swap(closestIntersection, newIntersection);
            continue;
        }
        if(*newIntersection < *closestIntersection) {
            std::swap(closestIntersection, newIntersection);
            continue;
        }
    }
    return closestIntersection;
}
