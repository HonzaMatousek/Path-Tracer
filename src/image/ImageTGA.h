#pragma once

#include "Image.h"

class ImageTGA : public Image {
public:
    ImageTGA(int width, int height);

    void Save(const std::string & fileName, double exposition) const override;
};
