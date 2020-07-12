#include "Color.h"
#include <algorithm>
#include <cmath>

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
    if(exposition == 0) {
        buffer[0] = (unsigned char)std::clamp<double>(r, 0, 255);
        buffer[1] = (unsigned char)std::clamp<double>(g, 0, 255);
        buffer[2] = (unsigned char)std::clamp<double>(b, 0, 255);
        return;
    }
    // use physically correct exposition (Reinhard's tone mapping?)
    buffer[0] = (unsigned char)std::clamp<double>(pow(r * exposition, 1/2.2) * 255, 0, 255);
    buffer[1] = (unsigned char)std::clamp<double>(pow(g * exposition, 1/2.2) * 255, 0, 255);
    buffer[2] = (unsigned char)std::clamp<double>(pow(b * exposition, 1/2.2) * 255, 0, 255);
}

void Color::WriteIntoByteBufferBGR(unsigned char *buffer, double exposition) const {
    if(exposition == 0) {
        buffer[2] = (unsigned char)std::clamp<double>(r, 0, 255);
        buffer[1] = (unsigned char)std::clamp<double>(g, 0, 255);
        buffer[0] = (unsigned char)std::clamp<double>(b, 0, 255);
        return;
    }
    // use physically correct exposition (Reinhard's tone mapping?)
    buffer[2] = (unsigned char)std::clamp<double>(pow(r * exposition, 1/2.2) * 255, 0, 255);
    buffer[1] = (unsigned char)std::clamp<double>(pow(g * exposition, 1/2.2) * 255, 0, 255);
    buffer[0] = (unsigned char)std::clamp<double>(pow(b * exposition, 1/2.2) * 255, 0, 255);
}

Color Color::ReadFromByteBufferRGB(unsigned char *buffer, double exposition) {
    if(exposition == 0) {
        return Color(
                buffer[0] / 255.0,
                buffer[1] / 255.0,
                buffer[2] / 255.0
        );
    }
    return Color(
            pow(buffer[0] / 255.0, 2.2) / exposition,
            pow(buffer[1] / 255.0, 2.2) / exposition,
            pow(buffer[2] / 255.0, 2.2) / exposition
    );
}

Vector3D Color::ToNormal() const {
    return Vector3D(
            (r * 2) - 1,
            (g * 2) - 1,
            (b * 2) - 1
    );
}
