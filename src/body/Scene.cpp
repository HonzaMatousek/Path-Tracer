//
// Created by honza on 24.05.20.
//

#include "Scene.h"

void Scene::AddBody(std::unique_ptr<Body> body) {
    bodies.emplace_back(std::move(body));
}

void Scene::Intersect(const Ray & ray, Intersection & intersection) const {
    for(const auto & body : bodies) {
        body->Intersect(ray, intersection);
    }
}
