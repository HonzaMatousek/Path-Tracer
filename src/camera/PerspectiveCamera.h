#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera {
    Vector3D eye;
    Vector3D direction;
    Vector3D up;
    Vector3D left;
    int width;
    int height;
    double fov;
public:
    PerspectiveCamera(Vector3D eye, Vector3D direction, Vector3D up, int width, int height, double fov);

    [[ nodiscard ]]
    virtual Ray Project(double w, double h) const;
};
