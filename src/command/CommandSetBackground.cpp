#include <memory>
#include "CommandSetBackground.h"
#include "../image/ImageJPEG.h"

void CommandSetBackground::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    std::string texturePath;
    lineStream >> texturePath;
    std::shared_ptr<Image> bg(new ImageJPEG(texturePath, 1, 95));
    bg->SetRepeatY(false);
    sceneBuilder.SetSceneMaterialInterpolator(
            std::make_unique<LegacyTextureInterpolator>(std::make_unique<SpherePolarInterpolator>(),
                                                        bg,
                                                        nullptr,
                                                        false,
                                                        0.0
            )
    );
}
