#pragma once

#include "Command.h"

class CommandCameraPerspective : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
