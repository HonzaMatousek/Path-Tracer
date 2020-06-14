#pragma once

#include "Image.h"

class ImageJPEG : public Image {
    int quality;
public:
    ImageJPEG(int width, int height, int quality);
    ImageJPEG(const std::string & fileName, double exposition, int quality);

    void Save(const std::string & fileName, double exposition) const override;
};
