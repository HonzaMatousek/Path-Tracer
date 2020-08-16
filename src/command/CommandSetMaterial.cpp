#include "CommandSetMaterial.h"

#include <istream>

void CommandSetMaterial::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    std::string newCurrentMaterialName;
    lineStream >> newCurrentMaterialName;
    sceneBuilder.current_material = sceneBuilder.GetMaterial(newCurrentMaterialName);
}
