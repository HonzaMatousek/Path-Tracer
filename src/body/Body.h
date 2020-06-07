#pragma once

#include "../math/Ray.h"
#include "../math/Intersection.h"
#include "../material/Material.h"

#include <memory>

class Body {
protected:
    Material material;
public:
    const Vector3D lowerCorner;
    const Vector3D upperCorner;

    explicit Body(const Material & material = Material(), const Vector3D & lowerCorner = Vector3D(0,0,0), const Vector3D & upperCorner = Vector3D(0,0,0)) : material(material), lowerCorner(lowerCorner), upperCorner(upperCorner) {}
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
};
