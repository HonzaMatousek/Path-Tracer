#include "CommandCameraSpherical.h"
#include "../camera/SphericalCamera.h"
#include "../math/Transform.h"

#include <istream>

void CommandCameraSpherical::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double eye_x, eye_y, eye_z;
    lineStream >> eye_x >> eye_y >> eye_z >> sceneBuilder.image_width >> sceneBuilder.image_height;
    sceneBuilder.camera = std::make_unique<SphericalCamera>(sceneBuilder.transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)), sceneBuilder.image_width, sceneBuilder.image_height);
}
