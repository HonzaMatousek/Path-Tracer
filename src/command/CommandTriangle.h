#pragma once

#include "Command.h"

class CommandTriangle : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
