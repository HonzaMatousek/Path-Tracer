#include <cmath>
#include <iostream>
#include <random>
#include "Marble.h"
#include "../math/Util.h"

double Marble::getVal(double x, double y, double z, unsigned int octave) const {
    x *= 1u << octave;
    y *= 1u << octave;
    z *= 1u << octave;
    int x0 = (int)x;
    if(x < 0) x0--;
    int x1 = x0 + 1;
    int y0 = (int)y;
    if(y < 0) y0--;
    int y1 = y0 + 1;
    int z0 = (int)z;
    if(z < 0) z0--;
    int z1 = z0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    double sx = x - (double)x0;
    double sy = y - (double)y0;
    double sz = z - (double)z0;

    int xt = x0 / 16;
    int yt = y0 / 16;
    int zt = z0 / 16;

    int shift = xt + yt + zt;

    x0 = (x0 + shift) & 0xf;
    x1 = (x1 + shift) & 0xf;
    y0 = (y0 + shift) & 0xf;
    y1 = (y1 + shift) & 0xf;
    z0 = (z0 + shift) & 0xf;
    z1 = (z1 + shift) & 0xf;

    double v0 = Util::Lerp(coefficients[x0][y0][z0], coefficients[x1][y0][z0], sx);
    double v1 = Util::Lerp(coefficients[x0][y1][z0], coefficients[x1][y1][z0], sx);
    double v2 = Util::Lerp(coefficients[x0][y0][z1], coefficients[x1][y0][z1], sx);
    double v3 = Util::Lerp(coefficients[x0][y1][z1], coefficients[x1][y1][z1], sx);
    double v4 = Util::Lerp(v0, v1, sy);
    double v5 = Util::Lerp(v2, v3, sy);
    return Util::Lerp(v4, v5, sz);
}

Color Marble::GetPixel(double x, double y, double z) const {
    x *= 10;
    y *= 10;
    z *= 10;
    double val = (getVal(x*0.89,y*0.89,z*0.89,0) * 4 + getVal(x*0.94,y*0.94,z*0.94,1) * 2 + getVal(x*1.02,y*1.02,z*1.02,2)) / 7;

    if(val < 0.4 || (val < 0.5 && val > 0.45) || (val < 0.6 && val > 0.57) || (val < 0.8 && val > 0.75)) {
        return Color(val, val, val) / 4;
    }

    return Color((1+val)/2, (1+val)/2, (1+val)/2);
}

bool Marble::isSpatial() const {
    return true;
}

void Marble::Save(const std::string &fileName, double exposition) const {}

Marble::Marble() : Image(0,0) {
    auto r = std::random_device();
    auto d = std::uniform_real_distribution(0.0, 1.0);
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            for(int k = 0; k < 16; k++) {
                coefficients[i][j][k] = d(r);
            }
        }
    }
}
