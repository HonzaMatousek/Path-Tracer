#pragma once

#include "Command.h"

class CommandCameraSpherical : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
