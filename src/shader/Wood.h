#pragma once

#include "../image/Image.h"

class Wood : public Image {
    //double randoms[64];
    //double randoms2[64];
    double coefficients[16][16][16];

    double getVal(double x, double y, double z, unsigned int octave) const;
public:
    Wood();

    Color GetPixel(double x, double y, double z) const override;

    bool isSpatial() const override;

    void Save(const std::string &fileName, double exposition) const override;
};
