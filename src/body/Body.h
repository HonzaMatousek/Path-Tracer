#pragma once

#include "../math/Ray.h"
#include "../math/Intersection.h"
#include "../material/Material.h"
#include "../interpolator/Interpolator.h"
#include "../math/Transform.h"

#include <memory>

class Body {
protected:
    std::unique_ptr<Interpolator<Material>> materialInterpolator;
    std::unique_ptr<Interpolator<Transform>> normalInterpolator;
public:
    const Vector3D lowerCorner;
    const Vector3D upperCorner;

    explicit Body(std::unique_ptr<Interpolator<Material>> && materialInterpolator = nullptr, std::unique_ptr<Interpolator<Transform>> && normalInterpolator = std::make_unique<NormalizeInterpolator>(), const Vector3D & lowerCorner = Vector3D(0,0,0), const Vector3D & upperCorner = Vector3D(0,0,0)) : materialInterpolator(std::move(materialInterpolator)), normalInterpolator(std::move(normalInterpolator)), lowerCorner(lowerCorner), upperCorner(upperCorner) {}
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
    Material GetMaterial(const Vector3D & localCoordinates) const {
        return materialInterpolator->Interpolate(localCoordinates);
    }

    [[ nodiscard ]]
    Transform GetNormal(const Vector3D & localCoordinates) const {
        return normalInterpolator->Interpolate(localCoordinates);
    }

    void SetNormalInterpolator(std::unique_ptr<Interpolator<Transform>> && normalInterpolator) {
        this->normalInterpolator.swap(normalInterpolator);
    }

    void SwapNormalInterpolator(std::unique_ptr<Interpolator<Transform>> & normalInterpolator) {
        std::swap(this->normalInterpolator, normalInterpolator);
    }

    void SetMaterialInterpolator(std::unique_ptr<Interpolator<Material>> && materialInterpolator) {
        this->materialInterpolator.swap(materialInterpolator);
    }
};
