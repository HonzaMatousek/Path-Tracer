#include "CommandCameraOrthogonal.h"
#include "../camera/OrthogonalCamera.h"
#include "../math/Transform.h"

#include <istream>

void CommandCameraOrthogonal::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double eye_x, eye_y, eye_z, dir_x, dir_y, dir_z, up_x, up_y, up_z, horizontalExtent;
    lineStream >> eye_x >> eye_y >> eye_z >> dir_x >> dir_y >> dir_z >> up_x >> up_y >> up_z >> sceneBuilder.image_width >> sceneBuilder.image_height >> horizontalExtent;
    sceneBuilder.camera = std::make_unique<OrthogonalCamera>(sceneBuilder.transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)),
                                                             sceneBuilder.transforms.top().ApplyWithoutTranslation(Vector3D(dir_x, dir_y, dir_z)),
                                                             sceneBuilder.transforms.top().ApplyWithoutTranslation(Vector3D(up_x, up_y, up_z)), sceneBuilder.image_width, sceneBuilder.image_height, horizontalExtent);
}
