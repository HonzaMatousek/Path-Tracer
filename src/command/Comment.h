#pragma once

#include "Command.h"

class Comment : public Command {
public:
    void Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const override;
};
