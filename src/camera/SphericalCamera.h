#pragma once

#include "Camera.h"

class SphericalCamera : public Camera {
    Vector3D eye;
    int width;
    int height;
public:
    SphericalCamera(const Vector3D & eye, int width, int height);

    [[ nodiscard ]]
    Ray Project(double w, double h) const override;
};
