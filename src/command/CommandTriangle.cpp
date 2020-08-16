#include "CommandTriangle.h"

#include <istream>
#include <memory>
#include "../math/Transform.h"
#include "../body/Triangle.h"
#include "../body/Scene.h"

void CommandTriangle::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double ax, ay, az, bx, by, bz, cx, cy, cz;
    lineStream >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz;
    sceneBuilder.scene.AddBody(std::make_unique<Triangle>(sceneBuilder.transforms.top().Apply(Vector3D(ax, ay, az)), sceneBuilder.transforms.top().Apply(Vector3D(bx, by, bz)), sceneBuilder.transforms.top().Apply(Vector3D(cx, cy, cz)), sceneBuilder.current_material->base));
}
