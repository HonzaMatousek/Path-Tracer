#pragma once

#include "Command.h"

class CommandLens : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
