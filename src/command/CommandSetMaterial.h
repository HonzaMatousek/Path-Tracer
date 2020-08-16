#pragma once

#include "Command.h"

class CommandSetMaterial : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
