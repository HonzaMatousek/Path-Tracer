#pragma once

#include <string>

class Transform;
class SceneBuilder;

class ModelOBJ {
public:
    static void Import(const std::string & fileName, const Transform & transform, SceneBuilder & sceneBuilder);
};
