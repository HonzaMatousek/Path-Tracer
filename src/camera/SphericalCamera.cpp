//
// Created by honza on 26.07.20.
//

#include "SphericalCamera.h"

SphericalCamera::SphericalCamera(const Vector3D &eye) : eye(eye) {}

Ray SphericalCamera::Project(double w, double h) const {
    double longitude = w / width * 2 * M_PI; // 0 ... 2pi // phi
    double latitude = h / height * M_PI; // 0  ...  +pi // theta
    auto direction = Vector3D(sin(latitude) * cos(longitude), cos(latitude), sin(latitude) * sin(longitude));
    return Ray(eye, direction);
}

void SphericalCamera::SetSensorSize(int newSensorWidth, int newSensorHeight) {
    width = newSensorWidth;
    height = newSensorHeight;
}
