#pragma once

#include "Command.h"

class CommandRotateY : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
