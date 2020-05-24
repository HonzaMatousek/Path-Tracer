#pragma once

#include <string>
#include <vector>
#include "../math/Color.h"

class Image {
protected:
    std::vector<std::vector<Color>> pixels;
public:
    const int width;
    const int height;

    Image(int width, int height);

    void AddPixel(int x, int y, const Color & color);

    virtual void Save(const std::string & fileName, double exposition) const = 0;
};
