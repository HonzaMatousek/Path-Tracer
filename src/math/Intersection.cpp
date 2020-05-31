#include <random>
#include "Intersection.h"

std::mt19937 generator;

Intersection::Intersection(const Vector3D &point, const Vector3D &normal, double t, const Material & material) : point(point), normal(normal), t(t), material(material) {
    this->normal.Normalize();
}

Ray Intersection::Reflect(const Ray &incoming, double& powerMultiplier) {
    Vector3D idealReflection = incoming.direction - normal * (2 * incoming.direction.Dot(normal));
    if(material.reflective) {
        return Ray(point, idealReflection);
    }
    else {
        std::uniform_real_distribution<double> d(-1.0,1.0);
        Vector3D randomDirection = Vector3D(d(generator), d(generator), d(generator));
        if(randomDirection.Dot(normal) < 0) {
            randomDirection *= -1;
        }
        powerMultiplier *= (randomDirection.Dot(incoming.direction) + 1) / 2;
        return Ray(point, randomDirection);
    }
}

bool Intersection::operator<(const Intersection &rhs) const {
    return t < rhs.t;
}
