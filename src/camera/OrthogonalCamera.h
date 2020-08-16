#pragma once

#include "Camera.h"

class OrthogonalCamera : public Camera {
    Vector3D eye;
    Vector3D direction;
    Vector3D up;
    Vector3D left;
    double horizontalExtent;
public:
    OrthogonalCamera(Vector3D eye, Vector3D direction, Vector3D up, double horizontalExtent);

    [[ nodiscard ]]
    Ray Project(double w, double h) const override;

    void SetSensorSize(int newSensorWidth, int newSensorHeight) override;
};

