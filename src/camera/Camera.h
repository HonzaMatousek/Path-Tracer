#pragma once

#include "../math/Vector3D.h"
#include "../math/Ray.h"
#include "../material/Environment.h"

class Camera {
    Vector3D eye;
    Vector3D direction;
    Vector3D up;
    Vector3D left;
    int width;
    int height;
    double fov;
public:
    Camera(Vector3D eye, Vector3D direction, Vector3D up, int width, int height, double fov);

    [[ nodiscard ]]
    Ray Project(double w, double h) const;

    Environment environment;
};
