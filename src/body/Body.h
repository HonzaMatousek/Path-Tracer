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

    [[ nodiscard ]]
    virtual std::unique_ptr<Intersection> Intersect(const Ray & ray) const = 0;
};
