#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include "Voronoi.h"

inline double lerp(double a0, double a1, double w) {
    return (1.0f - w)*a0 + w*a1;
}

double Voronoi::getVal(double x, double y, double z, unsigned int octave) const {
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
    if(x0 < 0) xt--;
    if(y0 < 0) yt--;
    if(z0 < 0) zt--;

    Vector3D p(x - xt * 16,y - yt * 16,z - zt * 16);
    //int shift = 0;//xt + yt + zt;

    //x0 = (x0 + shift) & 0xf;
    //x1 = (x1 + shift) & 0xf;
    //y0 = (y0 + shift) & 0xf;
    //y1 = (y1 + shift) & 0xf;
    //z0 = (z0 + shift) & 0xf;
    //z1 = (z1 + shift) & 0xf;

    double t = std::numeric_limits<double>::infinity();

    for(int i = x0 - 2; i < x0 + 2; i++)
        for(int j = y0 - 2; j < y0 + 2; j++)
            for(int k = z0 - 2; k < z0 + 2; k++) {
                auto p2 = points[i & 0xf][j & 0xf][k & 0xf];
                if(p2.x - p.x > 8) p2.x -= 16;
                if(p2.y - p.y > 8) p2.y -= 16;
                if(p2.z - p.z > 8) p2.z -= 16;
                if(p2.x - p.x < -8) p2.x += 16;
                if(p2.y - p.y < -8) p2.y += 16;
                if(p2.z - p.z < -8) p2.z += 16;
                t = std::min(t, p2.Distance(p));
            }

    return t;
}

Color Voronoi::GetPixel(double x, double y, double z) const {
    x *= 10;
    y *= 10;
    z *= 10;
    double val = pow(getVal(x,y,z,0) / std::sqrt(3.0), 2);

    /*if(val < 0.4 || (val < 0.5 && val > 0.45) || (val < 0.6 && val > 0.57) || (val < 0.8 && val > 0.75)) {
        return Color(val, val, val) / 4;
    }

    return Color((1+val)/2, (1+val)/2, (1+val)/2);*/
    return Color(val, val, val);
}

bool Voronoi::isSpatial() const {
    return true;
}

void Voronoi::Save(const std::string &fileName, double exposition) const {}

Voronoi::Voronoi() : Image(0,0) {
    auto r = std::random_device();
    auto d = std::uniform_real_distribution(0.0, 1.0);
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            for(int k = 0; k < 16; k++) {
                points[i][j][k] = Vector3D(i + d(r), j + d(r), k + d(r));
            }
        }
    }
}
