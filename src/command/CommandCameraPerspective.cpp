#include "CommandCameraPerspective.h"
#include "../camera/PerspectiveCamera.h"
#include "../math/Transform.h"

#include <istream>

void CommandCameraPerspective::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double eye_x, eye_y, eye_z, dir_x, dir_y, dir_z, up_x, up_y, up_z, fov;
    lineStream >> eye_x >> eye_y >> eye_z >> dir_x >> dir_y >> dir_z >> up_x >> up_y >> up_z >> sceneBuilder.image_width >> sceneBuilder.image_height >> fov;
    sceneBuilder.camera = std::make_unique<PerspectiveCamera>(sceneBuilder.transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)),
                                                 sceneBuilder.transforms.top().ApplyWithoutTranslation(Vector3D(dir_x, dir_y, dir_z)),
                                                 sceneBuilder.transforms.top().ApplyWithoutTranslation(Vector3D(up_x, up_y, up_z)), sceneBuilder.image_width, sceneBuilder.image_height, fov);
}
