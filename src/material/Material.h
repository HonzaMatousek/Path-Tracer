#pragma once

#include "Color.h"
#include "../math/Vector3D.h"

class Material {
public:
    Color emissive;
    Color albedo;
    bool metalness = false;
    double roughness = 0.0;
    double refractiveIndex = 1.0;
    double opacity = 1.0;
    Color attenuation = Color();
    double dispersionFactor = 1.0;

    Vector3D normal = Vector3D(0,0,1);

    Material() = default;
    Material(const Color & emissive, const Color & albedo, bool metalness = true, double roughness = 0.0, double refractiveIndex = INFINITY, double opacity = 1.0, const Vector3D & normal = Vector3D(0, 0, 1), const Color & attenuation = Color(), double dispersionFactor = 1.0) : emissive(emissive), albedo(albedo), metalness(metalness), roughness(roughness), refractiveIndex(refractiveIndex), opacity(opacity), normal(normal), attenuation(attenuation), dispersionFactor(dispersionFactor) {}

    [[ nodiscard ]]
    Material   operator +  (const Material & rhs) const;
    Material & operator += (const Material & rhs);

    [[ nodiscard ]]
    Material   operator -  (const Material & rhs) const;
    Material & operator -= (const Material & rhs);

    [[ nodiscard ]]
    Material   operator *  (double scale) const;
    Material & operator *= (double scale);
};
