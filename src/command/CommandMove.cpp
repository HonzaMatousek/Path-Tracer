#include "CommandMove.h"
#include "../math/Transform.h"

#include <iostream>

void CommandMove::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double x, y, z;
    lineStream >> x >> y >> z;
    sceneBuilder.transforms.push(sceneBuilder.transforms.top() * Transform::Translation(x, y, z));
}
