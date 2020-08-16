#pragma once

#include "Command.h"

class CommandSetBackground : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
