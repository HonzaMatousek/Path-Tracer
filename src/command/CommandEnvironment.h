#pragma once

#include "Command.h"

class CommandEnvironment : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
