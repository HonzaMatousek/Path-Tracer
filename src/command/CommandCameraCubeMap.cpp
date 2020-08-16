#include "CommandCameraCubeMap.h"
#include "../camera/CubeMapCamera.h"
#include "../math/Transform.h"

#include <istream>

void CommandCameraCubeMap::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double eye_x, eye_y, eye_z;
    lineStream >> eye_x >> eye_y >> eye_z >> sceneBuilder.image_width >> sceneBuilder.image_height;
    sceneBuilder.camera = std::make_unique<CubeMapCamera>(sceneBuilder.transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)), sceneBuilder.image_width, sceneBuilder.image_height);
}
