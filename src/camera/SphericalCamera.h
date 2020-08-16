#pragma once

#include "Camera.h"

class SphericalCamera : public Camera {
    Vector3D eye;
public:
    SphericalCamera(const Vector3D & eye);

    [[ nodiscard ]]
    Ray Project(double w, double h) const override;

    void SetSensorSize(int newSensorWidth, int newSensorHeight) override;
};
