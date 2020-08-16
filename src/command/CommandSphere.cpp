#include "CommandSphere.h"

#include <istream>
#include <memory>
#include "../math/Transform.h"
#include "../body/Sphere.h"
#include "../body/Scene.h"

void CommandSphere::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double x, y, z, radius;
    lineStream >> x >> y >> z >> radius;
    auto sphere = std::make_unique<Sphere>(sceneBuilder.transforms.top().Apply(Vector3D(x, y, z)), radius, sceneBuilder.current_material->base);
    if(sceneBuilder.current_material->albedoTexture && sceneBuilder.current_material->albedoTexture->isSpatial()) {
        sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                std::make_unique<PassThroughInterpolator>(),
                sceneBuilder.current_material
        ));
    }
    else {
        sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                std::make_unique<SpherePolarInterpolator>(),
                sceneBuilder.current_material
        ));
    }
    sceneBuilder.scene.AddBody(std::move(sphere));
}
