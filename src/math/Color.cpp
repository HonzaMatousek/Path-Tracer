#include "Color.h"

Color Color::operator+(const Color & rhs) const {
    return Color(r + rhs.r, g + rhs.g, b + rhs.b);
}

Color & Color::operator+=(const Color & rhs) {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
}

Color Color::operator-(const Color & rhs) const {
    return Color(r - rhs.r, g - rhs.g, b - rhs.b);
}

Color & Color::operator-=(const Color & rhs) {
    r -= rhs.r;
    g -= rhs.g;
    b -= rhs.b;
    return *this;
}

bool Color::operator==(const Color & rhs) const {
    // todo: use epsilon
    return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool Color::operator!=(const Color & rhs) const {
    return !(*this == rhs);
}

Color Color::operator*(double scale) const {
    return Color(r * scale, g * scale, b * scale);
}

Color &Color::operator*=(double scale) {
    r *= scale;
    g *= scale;
    b *= scale;
    return *this;
}

Color Color::operator/(double scale) const {
    return Color(r / scale, g / scale, b / scale);
}

Color &Color::operator/=(double scale) {
    r /= scale;
    g /= scale;
    b /= scale;
    return *this;
}

Color Color::operator*(const Color & rhs) const {
    return Color(r * rhs.r, g * rhs.g, b * rhs.b);
}

Color &Color::operator*=(const Color & rhs) {
    r *= rhs.r;
    g *= rhs.g;
    b *= rhs.b;
    return *this;
}

Color Color::operator/(const Color & rhs) const {
    return Color(r / rhs.r, g / rhs.g, b / rhs.b);
}

Color &Color::operator/=(const Color & rhs) {
    r /= rhs.r;
    g /= rhs.g;
    b /= rhs.b;
    return *this;
}

void Color::WriteIntoByteBufferRGB(unsigned char *buffer, double exposition) const {
    // use physically correct exposition (Reinhard's tone mapping?)
    buffer[0] = (unsigned char)(r * exposition);
    buffer[1] = (unsigned char)(g * exposition);
    buffer[2] = (unsigned char)(b * exposition);
}
