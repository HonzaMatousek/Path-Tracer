#include "CommandLens.h"

#include <istream>
#include <memory>
#include "../math/Transform.h"
#include "../body/Cylinder.h"
#include "../body/SphereEnd.h"
#include "../body/PlaneCircle.h"
#include "../body/Scene.h"

void CommandLens::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double radius, depth;
    lineStream >> radius >> depth;
    {
        auto cylinder = std::make_unique<Cylinder>(sceneBuilder.transforms.top().Apply(Vector3D(0, 0, 0)), sceneBuilder.transforms.top().ApplyWithoutTranslation(Vector3D(0, 0, 1)), radius, -depth / 2, depth / 2, sceneBuilder.current_material->base);
        if(sceneBuilder.current_material->albedoTexture && sceneBuilder.current_material->albedoTexture->isSpatial()) {
            cylinder->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                    std::make_unique<PassThroughInterpolator>(),
                    sceneBuilder.current_material
            ));
        }
        else {
            cylinder->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                    std::make_unique<SpherePolarInterpolator>(),
                    sceneBuilder.current_material
            ));
        }
        sceneBuilder.scene.AddBody(std::move(cylinder));
    }
    // read one end
    for(int i = 0; i < 2; i++) {
        std::string type;
        lineStream >> type;
        if(type == "flat") {
            sceneBuilder.scene.AddBody(std::make_unique<PlaneCircle>(sceneBuilder.transforms.top().Apply(Vector3D(0, 0, i == 0 ? -depth/2 : depth/2)), sceneBuilder.transforms.top().ApplyWithoutTranslation(Vector3D(0, 0, i == 0 ? -1 : 1)), radius, sceneBuilder.current_material->base));
        }
        else if(type == "sphere") {
            double curvatureRadius;
            lineStream >> curvatureRadius;
            double a = sqrt(curvatureRadius * curvatureRadius - radius * radius);
            double q = (std::abs(curvatureRadius) - a) * (std::signbit(curvatureRadius) ? -1 : 1);
            auto sphere = std::make_unique<SphereEnd>(sceneBuilder.transforms.top().Apply(Vector3D(0, 0, i == 0 ? -depth/2 - q : depth/2 + q)), curvatureRadius, sceneBuilder.transforms.top().ApplyWithoutTranslation(Vector3D(0, 0, i == 0 ? -1 : 1)), radius, sceneBuilder.current_material->base);
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
    }
}
