#pragma once

#include "Command.h"

class CommandLoadModel : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
