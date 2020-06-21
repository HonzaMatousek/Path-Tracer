#pragma once

#include "Model.h"

class ModelOBJ : public Model {
public:
    static void Import(const std::string & fileName, const Transform & transform, Scene & scene);
};
