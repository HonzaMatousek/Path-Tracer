#pragma once

#include "Color.h"

class Material {
public:
    Color emissive;
    Color albedo;
    bool reflective;

    Material() = default;
    Material(const Color & emissive, const Color & albedo, bool reflective = true) : emissive(emissive), albedo(albedo), reflective(reflective) {}
};
