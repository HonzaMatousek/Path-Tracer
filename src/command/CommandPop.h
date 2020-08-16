#pragma once

#include "Command.h"

class CommandPop : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
