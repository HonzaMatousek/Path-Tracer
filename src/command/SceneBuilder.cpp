//
// Created by honza on 16.08.20.
//

#include "SceneBuilder.h"

#include "../math/Transform.h"
#include "../body/Scene.h"
#include "../camera/SphericalCamera.h"
#include "../effect/Effect.h"

SceneBuilder::SceneBuilder(Scene &scene) : scene(scene) {
    transforms.push(Transform::Scale(1));
    current_material = &scene.materialLibrary["whiteDiffuse"];
    image_width = 512;
    image_height = 512;
    camera = std::make_unique<SphericalCamera>(Vector3D(0,0,0));
    effect = std::make_unique<Effect>();
}

TexturedMaterial *SceneBuilder::GetMaterial(const std::string &newMaterialName) const {
    auto it = scene.materialLibrary.find(newMaterialName);
    if(it == scene.materialLibrary.end()) {
        throw std::runtime_error("Material not found, GTFO!");
    }
    return &(it->second);
}

TexturedMaterial *SceneBuilder::NewMaterial(const std::string &newMaterialName) const {
    return &scene.materialLibrary[newMaterialName];
}

void SceneBuilder::SetSceneMaterialInterpolator(std::unique_ptr<Interpolator<Material>> && materialInterpolator) const {
    scene.SetMaterialInterpolator(std::move(materialInterpolator));
}

void SceneBuilder::CompileScene() const {
    scene.Compile();
}
