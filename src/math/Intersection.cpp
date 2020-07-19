#include <random>
#include <iostream>
#include "Intersection.h"
#include "../body/Body.h"

inline std::uniform_real_distribution<double> d(0.0, 1.0);

Vector3D RandomDirection(const Vector3D & usualDirection, std::mt19937 & generator, double spread) {
    if(spread == 0) {
        return usualDirection;
    }

    // generate random variables
    double u = d(generator);
    double v = d(generator);

    // map them to hemisphere
    const double r = sqrt(u * spread);
    const double theta = 2 * M_PI * v;

    const double x = r * cos(theta);
    const double y = r * sin(theta);

    Vector3D randomDirection (x, y, sqrt(1 - u * spread));

    return Transform::SomeBasisForZ(usualDirection).ApplyWithoutTranslation(randomDirection);
}

Ray Intersection::Reflect(const Ray &incoming, double& powerMultiplier, double& refractiveIndex, std::mt19937 & generator) {
    auto material = GetMaterial();
    auto normal = RandomDirection(GetNormal(material.normal), generator, material.roughness);

    // fresnel equation
    // Rs = pow(n1 cos phii - n2 cos phit / n1 cos phii + n2 cos phit, 2) // pricna polarizace
    // Rp = pow(n1 cos phit - n2 cos phii / n1 cos phit + n2 cos phii, 2) // podelna polarizaci

    double n1 = refractiveIndex;
    double cos_i = normal.Dot(incoming.direction); // cosi
    double sin_i = sqrt(1 - cos_i * cos_i);
    double refraction_ratio;
    if(cos_i > 0) {
        // going from inside out
        refraction_ratio = material.refractiveIndex;
        refractiveIndex = 1;
        normal *= -1;
        powerMultiplier *= pow((1 - material.opacity), t);
    }
    else {
        // going from outside in
        refraction_ratio = refractiveIndex / material.refractiveIndex;
        refractiveIndex = material.refractiveIndex;
        cos_i *= -1;
    }
    double sini = sqrt(1 - cos_i * cos_i);
    if(sini > 1 / refraction_ratio) {
        refractiveIndex = material.refractiveIndex;
        Vector3D idealReflection = incoming.direction - normal * (2 * incoming.direction.Dot(normal));
        return Ray(incoming.Point(t - 0.00000001), idealReflection);
    }
    double sin_t = refraction_ratio * sini;
    double cos_t = sqrt(1 - sin_t * sin_t);
    double n2 = refractiveIndex;

    double rs = pow((n1 * cos_i - n2 * cos_t) / (n1 * cos_i + n2 * cos_t), 2); // reflection in s mode
    double rp = pow((n1 * cos_t - n2 * cos_i) / (n1 * cos_t + n2 * cos_i), 2); // reflection in p mode

    double reflection = (rs + rp) / 2;
    double transmission = 1 - reflection;

    if(material.metalness) {
        //Vector3D normal = RandomDirection(normal, generator, 0.0);
        powerMultiplier *= reflection;
        Vector3D idealReflection = incoming.direction - normal * (2 * incoming.direction.Dot(normal));
        return Ray(incoming.Point(t - 0.00000001), idealReflection);
    }
    else {
        if(d(generator) <= reflection) {
            // reflection
            powerMultiplier *= reflection;
            Vector3D idealReflection = incoming.direction - normal * (2 * incoming.direction.Dot(normal));
            return Ray(incoming.Point(t - 0.00000001), idealReflection);
        }
        else {
            // transmission
            if(d(generator) >= material.opacity) {
                // refract
                Vector3D refracted = normal * -1 * sqrt(1 - sin_t * sin_t) + normal.Cross(incoming.direction).Cross(normal).Normalize() * sin_t;
                return Ray(incoming.Point(t + 0.00000001), refracted);
            }
            else {
                // diffuse
                auto randomDirection = RandomDirection(normal, generator, 1.0);
                powerMultiplier *= randomDirection.Dot(normal);
                return Ray(incoming.Point(t - 0.00000001), randomDirection);
            }
        }
    }
}

Material Intersection::GetMaterial() const {
    return body->GetMaterial(localCoordinates);
}

Vector3D Intersection::GetNormal(const Vector3D & materialNormal) const {
    return body->GetNormal(localCoordinates).ApplyWithoutTranslation(materialNormal).Normalize();
}
