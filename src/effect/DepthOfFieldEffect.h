#pragma once

#include "Effect.h"

class DepthOfFieldEffect : public Effect {
    double focus;
    double spread;
public:
    DepthOfFieldEffect(double focus, double spread = 0.01);

    void ModifyRay(Ray &ray, std::mt19937 & generator) const override;
};
