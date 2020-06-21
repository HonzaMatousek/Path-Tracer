#pragma once

#include <string>
#include "../body/Scene.h"
#include "../math/Transform.h"

class Model {
public:
    static void Import(const std::string & fileName, const Transform & transform, Scene & scene) {};
};
