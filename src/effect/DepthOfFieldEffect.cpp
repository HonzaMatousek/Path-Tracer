//
// Created by honza on 26.07.20.
//

#include "DepthOfFieldEffect.h"
#include "../math/Transform.h"

/* todo: move to common library */
inline std::uniform_real_distribution<double> d(0.0, 1.0);

inline Vector3D RandomDirection(const Vector3D & usualDirection, std::mt19937 & generator, double spread) {
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

void DepthOfFieldEffect::ModifyRay(Ray &ray, std::mt19937 & generator) const {
    Effect::ModifyRay(ray, generator);
    Vector3D point = ray.Point(focus);
    Vector3D direction = RandomDirection(ray.direction, generator, spread).Normalize();
    ray = Ray(point - direction * focus, direction);
}

DepthOfFieldEffect::DepthOfFieldEffect(double focus, double spread) : focus(focus), spread(spread) {}
