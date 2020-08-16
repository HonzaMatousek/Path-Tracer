#pragma once

#include <stack>
#include <string>
#include <memory>
#include "../interpolator/Interpolator.h"
#include "../camera/Camera.h"
#include "../effect/Effect.h"

class Scene;
class Transform;
class TexturedMaterial;
class Material;

class SceneBuilder {
public:
    Scene & scene;
    std::stack<Transform> transforms;
    TexturedMaterial * current_material;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Effect> effect;
    std::unique_ptr<Image> image;
    int image_width;
    int image_height;

    explicit SceneBuilder(Scene & scene);
    TexturedMaterial * GetMaterial(const std::string & newMaterialName) const;
    TexturedMaterial * NewMaterial(const std::string & newMaterialName) const;
    void SetSceneMaterialInterpolator(std::unique_ptr<Interpolator<Material>> && materialInterpolator) const;
    void CompileScene() const;
};
