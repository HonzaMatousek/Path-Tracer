#include <algorithm>
#include "Image.h"

Image::Image(int width, int height) : width(width), height(height) {
    pixels.resize(height);
    for(auto & line : pixels) {
        line.resize(width);
    }
}

void Image::AddPixel(int x, int y, const Color &color) {
    pixels[y][x] += color;
}

Color Image::GetPixel(double x, double y) const {
    return pixels[std::clamp(y, 0.0, 1.0) * (height - 1)][std::clamp(x, 0.0, 1.0) * (width - 1)];
}
