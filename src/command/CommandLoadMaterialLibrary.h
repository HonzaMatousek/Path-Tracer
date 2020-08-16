#pragma once

#include "Command.h"

class CommandLoadMaterialLibrary : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
