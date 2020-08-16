#include "CommandRotateY.h"

#include <istream>
#include "../math/Transform.h"

void CommandRotateY::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double deg;
    lineStream >> deg;
    sceneBuilder.transforms.push(sceneBuilder.transforms.top() * Transform::RotateY(deg / 180.0 * M_PI));
}
