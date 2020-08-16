#pragma once

#include "Command.h"

class CommandCameraOrthogonal : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
