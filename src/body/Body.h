#pragma once

#include "../math/Ray.h"
#include "../math/Intersection.h"

#include <memory>

class Body {
public:
    virtual ~Body() = default;

    [[ nodiscard ]]
    virtual std::unique_ptr<Intersection> Intersect(const Ray & ray) const = 0;
};
