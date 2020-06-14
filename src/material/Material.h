#pragma once

#include "Color.h"

class Material {
public:
    Color emissive;
    Color albedo;
    bool reflective;

    Material() = default;
    Material(const Color & emissive, const Color & albedo, bool reflective = true) : emissive(emissive), albedo(albedo), reflective(reflective) {}

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
