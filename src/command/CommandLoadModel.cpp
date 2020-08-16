#include "CommandLoadModel.h"
#include "../math/Transform.h"
#include "../model/ModelOBJ.h"

#include <iostream>

void CommandLoadModel::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    std::string modelPath;
    lineStream >> modelPath;
    ModelOBJ::Import(modelPath, sceneBuilder.transforms.top(), sceneBuilder);
}
