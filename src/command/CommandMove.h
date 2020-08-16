#pragma once

#include "Command.h"

class CommandMove : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
