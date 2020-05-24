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
