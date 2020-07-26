#pragma once

#include "../math/Ray.h"
#include <random>

class Effect {
public:
    virtual void ModifyRay(Ray& ray, std::mt19937 & generator) const {}
};
