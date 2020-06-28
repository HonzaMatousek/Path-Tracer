#pragma once

#include "Color.h"

class Material {
public:
    Color emissive;
    Color albedo;
    bool reflective = false;
    double roughness = 0.0;
    double refractiveIndex = 1.0;
    double opacity = 1.0;

    Material() = default;
    Material(const Color & emissive, const Color & albedo, bool reflective = true, double roughness = 0.0, double refractiveIndex = 1.0, double opacity = 1.0) : emissive(emissive), albedo(albedo), reflective(reflective), roughness(roughness), refractiveIndex(refractiveIndex), opacity(opacity) {}

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
