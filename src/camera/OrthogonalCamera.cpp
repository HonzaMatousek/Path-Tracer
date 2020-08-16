//
// Created by honza on 26.07.20.
//

#include "OrthogonalCamera.h"

OrthogonalCamera::OrthogonalCamera(Vector3D eye, Vector3D direction, Vector3D up, double horizontalExtent) : eye(eye), direction(direction.Normalize()), up(up), left(up.Cross(direction)), horizontalExtent(horizontalExtent)  {}

Ray OrthogonalCamera::Project(double w, double h) const {
    double wp = 1 - (w / width) * 2;
    double hp = 1 - (h / height) * 2;
    return Ray(eye + left * wp + up * hp, direction);
}

void OrthogonalCamera::SetSensorSize(int newSensorWidth, int newSensorHeight) {
    width = newSensorWidth;
    height = newSensorHeight;
    up.Normalize() *= horizontalExtent / 2 * height / width;
    left.Normalize() *= horizontalExtent / 2;
}
