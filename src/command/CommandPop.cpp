#include "CommandPop.h"
#include "../math/Transform.h"

#include <iostream>

void CommandPop::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    if(sceneBuilder.transforms.size() == 1) {
        throw std::runtime_error("Popping poop, GTFO!");
    }
    sceneBuilder.transforms.pop();
}
