#include "CommandEnvironment.h"

#include "../camera/Camera.h"

void CommandEnvironment::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    std::string newCurrentMaterialName;
    lineStream >> newCurrentMaterialName;
    TexturedMaterial * material = sceneBuilder.GetMaterial(newCurrentMaterialName);
    sceneBuilder.camera->environment.attenuation = material->base.attenuation;
    sceneBuilder.camera->environment.refractiveIndex = material->base.refractiveIndex;
}
