#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera {
    Vector3D eye;
    Vector3D direction;
    Vector3D up;
    Vector3D left;
    double fov;
public:
    PerspectiveCamera(Vector3D eye, Vector3D direction, Vector3D up, double fov);

    [[ nodiscard ]]
    Ray Project(double w, double h) const override;

    void SetSensorSize(int newSensorWidth, int newSensorHeight) override;
};
