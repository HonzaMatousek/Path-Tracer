#include <random>
#include <iostream>
#include "Intersection.h"
#include "../body/Body.h"

inline std::uniform_real_distribution<double> d(0.0, 1.0);

Vector3D RandomDirection(const Vector3D & usualDirection, std::mt19937 & generator, double spread) {
    // generate random variables
    double u = d(generator);
    double v = d(generator);

    // map them to hemisphere
    const double r = sqrt(u * spread);
    const double theta = 2 * M_PI * v;

    const double x = r * cos(theta);
    const double y = r * sin(theta);

    Vector3D randomDirection (x, y, sqrt(1 - u * spread));

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

Ray Intersection::Reflect(const Ray &incoming, double& powerMultiplier, double& refractiveIndex, std::mt19937 & generator) {
    auto material = GetMaterial();
    auto normal = RandomDirection(GetNormal(), generator, material.roughness);
    if(material.reflective) {
        //Vector3D normal = RandomDirection(normal, generator, 0.0);
        Vector3D idealReflection = incoming.direction - normal * (2 * incoming.direction.Dot(normal));
        return Ray(incoming.Point(t - 0.00000001), idealReflection);
    }
    else {
        if(d(generator) >= material.opacity) {
            // refract
            double ni = normal.Dot(incoming.direction); // cosi
            double mi;
            if(ni > 0) {
                // going from inside out
                mi = material.refractiveIndex;
                refractiveIndex = 1;
                normal *= -1;
                powerMultiplier *= pow((1 - material.opacity), t);
            }
            else {
                // going from outside in
                mi = refractiveIndex / material.refractiveIndex;
                refractiveIndex = material.refractiveIndex;
                ni *= -1;
            }
            double sini = sqrt(1 - ni*ni);
            if(sini > 1 / mi) {
                refractiveIndex = material.refractiveIndex;
                Vector3D idealReflection = incoming.direction - normal * (2 * incoming.direction.Dot(normal));
                return Ray(incoming.Point(t - 0.00000001), idealReflection);
            }
            double sint = mi * sini;
            Vector3D refracted = normal * -1 * sqrt(1 - sint * sint) + normal.Cross(incoming.direction).Cross(normal).Normalize() * sint;
            return Ray(incoming.Point(t + 0.00000001), refracted);
        }
        else {
            auto randomDirection = RandomDirection(normal, generator, 1.0);
            powerMultiplier *= randomDirection.Dot(normal);
            return Ray(incoming.Point(t - 0.00000001), randomDirection);
        }
    }
}

Material Intersection::GetMaterial() const {
    return body->GetMaterial(localCoordinates);
}

Vector3D Intersection::GetNormal() const {
    return body->GetNormal(localCoordinates);
}