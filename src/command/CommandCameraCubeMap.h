#pragma once

#include "Command.h"

class CommandCameraCubeMap : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
