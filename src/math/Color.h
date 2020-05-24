#pragma once

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
};
