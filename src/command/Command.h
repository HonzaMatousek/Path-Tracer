#pragma once

#include <iosfwd>
#include "SceneBuilder.h"

class Command {
public:
    virtual void Execute(SceneBuilder & sceneBuilder, std::istream & lineStream) const = 0;
};
