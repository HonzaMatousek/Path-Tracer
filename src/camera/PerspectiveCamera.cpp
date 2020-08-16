//
// Created by honza on 24.05.20.
//

#include "PerspectiveCamera.h"

#include <cmath>

PerspectiveCamera::PerspectiveCamera(Vector3D eye, Vector3D direction, Vector3D up, double fov) : eye(eye), direction(direction), up(up), left(up.Cross(direction)), fov(fov)  {}

Ray PerspectiveCamera::Project(double w, double h) const {
    //0,0 => up + right + direction
    //width, height => - up - right + direction

    // 0 - width => 1 - -1

    double wp = 1 - (w / width) * 2;
    double hp = 1 - (h / height) * 2;

    return Ray(eye, up * hp + left * wp + direction);
}

void PerspectiveCamera::SetSensorSize(int newSensorWidth, int newSensorHeight) {
    width = newSensorWidth;
    height = newSensorHeight;
    up.Normalize() *= height;
    left.Normalize() *= width;
    direction.Normalize() *= width / std::tan(fov * M_PI / 360);
}
