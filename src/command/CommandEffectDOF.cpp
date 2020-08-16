#include "CommandEffectDOF.h"
#include "../effect/Effect.h"
#include "../effect/DepthOfFieldEffect.h"

#include <istream>

void CommandEffectDOF::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    double focus, spread;
    lineStream >> focus >> spread;
    if(focus == 0 || spread == 0) {
        sceneBuilder.effect = std::make_unique<Effect>();
    }
    else {
        sceneBuilder.effect = std::make_unique<DepthOfFieldEffect>(focus, spread);
    }
}
