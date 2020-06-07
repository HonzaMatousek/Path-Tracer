#include <cmath>
#include "Vector3D.h"

const std::initializer_list<Vector3D::Axis> Vector3D::axes = {
    &Vector3D::x,
    &Vector3D::y,
    &Vector3D::z,
};
