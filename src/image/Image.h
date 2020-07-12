#pragma once

#include <string>
#include <vector>
#include "../material/Color.h"

class Image {
protected:
    std::vector<std::vector<Color>> pixels;
    bool repeatX = true;
    bool repeatY = true;
public:
    int width;
    int height;

    Image(int width, int height);

    void AddPixel(int x, int y, const Color & color);

    virtual Color GetPixel(double x, double y, double z = 0) const;

    [[ nodiscard ]]
    virtual bool isSpatial() const { return false; }

    virtual void Save(const std::string & fileName, double exposition) const = 0;
};
