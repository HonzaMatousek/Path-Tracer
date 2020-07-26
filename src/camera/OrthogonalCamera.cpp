//
// Created by honza on 26.07.20.
//

#include "OrthogonalCamera.h"

OrthogonalCamera::OrthogonalCamera(Vector3D eye, Vector3D direction, Vector3D up, int width, int height, double horizontalExtent) : eye(eye), direction(direction), up(up), left(up.Cross(direction)), width(width), height(height), horizontalExtent(horizontalExtent)  {
    this->up.Normalize() *= horizontalExtent / 2 * height / width;
    this->left.Normalize() *= horizontalExtent / 2;
    this->direction.Normalize();
}

Ray OrthogonalCamera::Project(double w, double h) const {
    double wp = 1 - (w / width) * 2;
    double hp = 1 - (h / height) * 2;
    return Ray(eye + left * wp + up * hp, direction);
}
