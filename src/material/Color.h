#pragma once

#include "../math/Vector3D.h"

const double speedOfLight = 299792458;
const double frequencyR = speedOfLight / 608e-9;
const double frequencyG = speedOfLight / 552e-9;
const double frequencyB = speedOfLight / 463e-9;

class Color {
public:
    double r;
    double g;
    double b;

    Color() : r(0), g(0), b() {};
    Color(double r, double g, double b) : r(r), g(g), b(b) {};

    [[ nodiscard ]]
    Color   operator +  (const Color & rhs) const;
    Color & operator += (const Color & rhs);
    [[ nodiscard ]]
    Color   operator -  (const Color & rhs) const;
    Color & operator -= (const Color & rhs);

    [[ nodiscard ]]
    bool       operator == (const Color & rhs) const;

    [[ nodiscard ]]
    bool       operator != (const Color & rhs) const;

    [[ nodiscard ]]
    Color   operator *  (double scale) const;
    Color & operator *= (double scale);
    [[ nodiscard ]]
    Color   operator /  (double scale) const;
    Color & operator /= (double scale);

    [[ nodiscard ]]
    Color   operator *  (const Color & rhs) const;
    Color & operator *= (const Color & rhs);
    [[ nodiscard ]]
    Color   operator /  (const Color & rhs) const;
    Color & operator /= (const Color & rhs);

    void WriteIntoByteBufferRGB(unsigned char * buffer, double exposition) const;
    void WriteIntoByteBufferBGR(unsigned char * buffer, double exposition) const;

    static Color ReadFromByteBufferRGB(unsigned char * buffer, double exposition);

    Vector3D ToNormal() const;

    [[ nodiscard ]]
    Color Exp() const;

    static Color FromFrequency(double frequency);
};
