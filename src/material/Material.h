#pragma once

#include "Color.h"

class Material {
public:
    Color emissive;
    Color albedo;

    Material() = default;
    Material(const Color & emissive, const Color & albedo) : emissive(emissive), albedo(albedo) {}
};
