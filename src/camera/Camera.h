#pragma once

#include "../math/Ray.h"
#include "../material/Environment.h"

class Camera {
public:
    [[ nodiscard ]]
    virtual Ray Project(double w, double h) const = 0;

    Environment environment;
};
