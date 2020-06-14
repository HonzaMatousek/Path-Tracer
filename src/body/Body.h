#pragma once

#include "../math/Ray.h"
#include "../math/Intersection.h"
#include "../material/Material.h"
#include "../interpolator/Interpolator.h"

#include <memory>

class Body {
protected:
    Material material;
    std::unique_ptr<Interpolator<Vector3D>> normalInterpolator;
public:
    const Vector3D lowerCorner;
    const Vector3D upperCorner;

    explicit Body(const Material & material = Material(), std::unique_ptr<Interpolator<Vector3D>> && normalInterpolator = std::make_unique<NormalizeInterpolator>(), const Vector3D & lowerCorner = Vector3D(0,0,0), const Vector3D & upperCorner = Vector3D(0,0,0)) : material(material), normalInterpolator(std::move(normalInterpolator)), lowerCorner(lowerCorner), upperCorner(upperCorner) {}
    virtual ~Body() = default;

    virtual void Intersect(const Ray & ray, Intersection & intersection) const = 0;

    [[ nodiscard ]]
    virtual bool IntersectAABB(const Vector3D & l, const Vector3D & u) const {
        for(auto axis : Vector3D::axes) {
            if(l.*axis > upperCorner.*axis) return false;
            if(u.*axis < lowerCorner.*axis) return false;
        }
        return true;
    }

    [[ nodiscard ]]
    virtual Material GetMaterial(const Vector3D & localCoordinates) const {
        return material;
    }

    [[ nodiscard ]]
    Vector3D GetNormal(const Vector3D & localCoordinates) const {
        return normalInterpolator->Interpolate(localCoordinates);
    }
};
