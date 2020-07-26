#pragma once

#include "Camera.h"

class CubeMapCamera : public Camera {
    Vector3D eye;
    double width;
    double height;
public:
    CubeMapCamera(const Vector3D & eye, int width, int height);

    [[ nodiscard ]]
    Ray Project(double w, double h) const override;
};
