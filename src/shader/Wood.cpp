#include <cmath>
#include <iostream>
#include <random>
#include "Wood.h"

inline double lerp(double a0, double a1, double w) {
    return (1.0f - w)*a0 + w*a1;
}

double Wood::getVal(double x, double y, double z, unsigned int octave) const {
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

    x0 &= 0xf;
    x1 &= 0xf;
    y0 &= 0xf;
    y1 &= 0xf;
    z0 &= 0xf;
    z1 &= 0xf;

    double v0 = lerp(coefficients[x0][y0][z0], coefficients[x1][y0][z0], sx);
    double v1 = lerp(coefficients[x0][y1][z0], coefficients[x1][y1][z0], sx);
    double v2 = lerp(coefficients[x0][y0][z1], coefficients[x1][y0][z1], sx);
    double v3 = lerp(coefficients[x0][y1][z1], coefficients[x1][y1][z1], sx);
    double v4 = lerp(v0, v1, sy);
    double v5 = lerp(v2, v3, sy);
    return lerp(v4, v5, sz);
}

Color Wood::GetPixel(double x, double y, double z) const {
    x *= 10;
    y *= 10;
    z *= 10;
    double val = (getVal(x,y,z,0) * 4 + getVal(x,y,z,1) * 2 + getVal(x,y,z,2)) / 7;

    double distance = std::pow(x*x + z*z, 0.6) * 10;
    //double angle = atan2(x, z) / M_PI * 32;

    //double val2 = (getVal(x,z,angle,0) * 4 + getVal(x,z,angle,1) * 2 + getVal(x,z,angle,2)) / 7;
    //return Color(val2, val2, val2);

    distance += getVal(0,0,distance * 0.5,0) / 3;

    distance += y;

    distance += val / 1.5;// / scale;// * cos(angle * (i + 1) + randoms2[i] + randoms[i] * scale) * randoms[i] * scale;

    double cosine = std::cos(distance);

    if(cosine > 0.8) {
        return Color(0.2,0.12,0.04) * (0.2 + cosine * 2);// + Color(0.3, 0.3, 0.3) * (val2 > 0.8 ? 1 : 0);
    }
    else {
        return Color(1,0.5,0.1) * (1.6 - (cosine + 5) / 5.8);// + Color(0.3, 0.3, 0.3) * (val2 > 0.8 ? 1 : 0);
    }
}

bool Wood::isSpatial() const {
    return true;
}

void Wood::Save(const std::string &fileName, double exposition) const {}

Wood::Wood() : Image(0,0) {
    auto r = std::random_device();
    auto d = std::normal_distribution(1.0, 0.2);
    auto d2 = std::uniform_real_distribution(0.0, 2 * M_PI);
    /*for(double & random : randoms) {
        random = d(r);
    }
    for(double & random : randoms2) {
        random = d2(r);
    }*/
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            for(int k = 0; k < 16; k++) {
                coefficients[i][j][k] = d2(r);
            }
        }
    }
}
