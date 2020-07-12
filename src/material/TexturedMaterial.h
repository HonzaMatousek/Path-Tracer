#pragma once

#include <memory>
#include "Material.h"
#include "../image/Image.h"

class TexturedMaterial {
public:
    Material base;
    std::shared_ptr<Image> emissiveTexture;
    std::shared_ptr<Image> albedoTexture;
    std::shared_ptr<Image> metalnessTexture;
    std::shared_ptr<Image> roughnessTexture;
    std::shared_ptr<Image> normalTexture;
    Material GetMaterial(double u, double v) const;
};
