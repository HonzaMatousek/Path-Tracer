#include <random>
#include "Intersection.h"

Intersection::Intersection(const Vector3D &point, const Vector3D &normal, double t, const Material & material) : point(point), normal(normal), t(t), material(material) {
    this->normal.Normalize();
}

Ray Intersection::Reflect(const Ray &incoming, double& powerMultiplier, std::mt19937 & generator) {
    Vector3D idealReflection = incoming.direction - normal * (2 * incoming.direction.Dot(normal));
    if(material.reflective) {
        return Ray(point, idealReflection);
    }
    else {
        std::uniform_real_distribution<double> d(0.0,1.0);
        double u = d(generator);
        double v = d(generator);
        double sin_th = sin(2 * M_PI * u);
        double cos_ph = 2 *  v - 1;
        // th = 2 * pi * u
        // phi = arccos (2 * v - 1)
        // x = sin th * cos phi
        // y = sin th * sin phi
        // z = cos th
        Vector3D randomDirection = Vector3D(
                sin_th * cos_ph,
                sin_th * sqrt(1 - cos_ph * cos_ph),
                sqrt(1 - sin_th * sin_th)
        );
        if (randomDirection.Dot(normal) < 0) {
            randomDirection *= -1;
        }
        powerMultiplier *= randomDirection.Dot(normal);
        return Ray(point, randomDirection);
    }
}

bool Intersection::operator<(const Intersection &rhs) const {
    return t < rhs.t;
}
