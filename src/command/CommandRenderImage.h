#pragma once

#include "Command.h"

class LineEmployer;

class CommandRenderImage : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
