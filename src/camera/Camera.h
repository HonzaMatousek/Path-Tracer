#pragma once

#include "../math/Ray.h"
#include "../material/Environment.h"

class Camera {
protected:
    int width = 1;
    int height = 1;
public:
    [[ nodiscard ]]
    virtual Ray Project(double w, double h) const = 0;

    virtual void SetSensorSize(int newSensorWidth, int newSensorHeight) = 0;

    Environment environment;
};
