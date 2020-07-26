//
// Created by honza on 26.07.20.
//

#include "SphericalCamera.h"

SphericalCamera::SphericalCamera(const Vector3D &eye, int width, int height) : eye(eye), width(width), height(height) {}

Ray SphericalCamera::Project(double w, double h) const {
    double longitude = w / width * 2 * M_PI; // 0 ... 2pi // phi
    double latitude = h / height * M_PI; // 0  ...  +pi // theta
    auto direction = Vector3D(sin(latitude) * cos(longitude), cos(latitude), sin(latitude) * sin(longitude));
    return Ray(eye, direction);
}
