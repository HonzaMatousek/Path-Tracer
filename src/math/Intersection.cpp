#include <random>
#include "Intersection.h"

Intersection::Intersection(const Vector3D &point, const Vector3D &normal, double t, const Material &material) : point(point), normal(normal), t(t), material(material) {
    this->normal.Normalize();
}

Vector3D RandomDirection(const Vector3D & usualDirection, std::mt19937 & generator) {
    // generate random variables
    static std::uniform_real_distribution<double> d(0.0, 1.0);
    double u = d(generator);
    double v = d(generator);

    // map them to hemisphere
    const double r = sqrt(u);
    const double theta = 2 * M_PI * v;

    const double x = r * cos(theta);
    const double y = r * sin(theta);

    Vector3D randomDirection (x, y, sqrt(1 - u));

    // reorient (0, 0, 1) to usualDirection
    if(usualDirection == Vector3D(0, 0, -1)) {
        randomDirection.z *= -1;
    }
    else if(usualDirection != Vector3D(0,0,1)) {
        // create orthonormal basis (a|b|usualDirection)
        auto a = usualDirection.Cross(Vector3D(0,0,1));
        a.Normalize();
        auto b = usualDirection.Cross(a);
        // rotate: (a|b|usualDirection) . randomDirection
        randomDirection = Vector3D(
                a.x * randomDirection.x + b.x * randomDirection.y + usualDirection.x * randomDirection.z,
                a.y * randomDirection.x + b.y * randomDirection.y + usualDirection.y * randomDirection.z,
                a.z * randomDirection.x + b.z * randomDirection.y + usualDirection.z * randomDirection.z
        );
    }

    return randomDirection;
}

Ray Intersection::Reflect(const Ray &incoming, double& powerMultiplier, std::mt19937 & generator) {
    Vector3D idealReflection = incoming.direction - normal * (2 * incoming.direction.Dot(normal));
    if(material.reflective) {
        return Ray(point, idealReflection);
    }
    else {
        auto randomDirection = RandomDirection(normal, generator);
        powerMultiplier *= randomDirection.Dot(normal);
        return Ray(point, randomDirection);
    }
}
