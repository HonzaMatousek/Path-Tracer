#pragma once

#include "Command.h"

class CommandEffectDOF : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
