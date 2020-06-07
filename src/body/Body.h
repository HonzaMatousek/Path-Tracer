#pragma once

#include "../math/Ray.h"
#include "../math/Intersection.h"
#include "../material/Material.h"

#include <memory>

class Body {
protected:
    Material material;
public:
    explicit Body(const Material & material = Material()) : material(material) {}
    virtual ~Body() = default;

    virtual void Intersect(const Ray & ray, Intersection & intersection) const = 0;
};
