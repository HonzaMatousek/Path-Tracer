#include <algorithm>
#include "Image.h"
#include "../math/Util.h"

size_t GetCoordinate(double in, int size, bool repeat, bool roundDown) {
    if(repeat) {
        return (roundDown ? floor : ceil)(Util::Wrap(in) * (size - 1));
    }
    else {
        return (roundDown ? floor : ceil)(std::clamp(in, 0.0, 1.0) * (size - 1));
    }
}

double GetFractionalPart(double in, int size, bool repeat) {
    return std::clamp(in, 0.0, 1.0) * (size - 1) - (size_t)(std::clamp(in, 0.0, 1.0) * (size - 1));
}

Image::Image(int width, int height) : width(width), height(height) {
    pixels.resize(height);
    for(auto & line : pixels) {
        line.resize(width);
    }
}

void Image::AddPixel(int x, int y, const Color &color) {
    pixels[y][x] += color;
}

Color Image::GetPixel(double x, double y, double z) const {
    auto x0 = GetCoordinate(x, width, repeatX, true);
    auto x1 = GetCoordinate(x, width, repeatX, false);
    auto y0 = GetCoordinate(y, height, repeatY, true);
    auto y1 = GetCoordinate(y, height, repeatY, false);

    auto c00 = pixels[y0][x0];
    auto c01 = pixels[y0][x1];
    auto c10 = pixels[y1][x0];
    auto c11 = pixels[y1][x1];
    auto c0 = Util::Lerp(c00, c01, GetFractionalPart(x, width, repeatX));
    auto c1 = Util::Lerp(c10, c11, GetFractionalPart(x, width, repeatX));
    return Util::Lerp(c0, c1, GetFractionalPart(y, height, repeatY));
}
