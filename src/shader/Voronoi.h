#pragma once

#include "../math/Vector3D.h"
#include "../material/Color.h"
#include "../image/Image.h"

class Voronoi : public Image {
    Vector3D points[16][16][16];

    double getVal(double x, double y, double z, unsigned int octave) const;
public:
    Voronoi();

    Color GetPixel(double x, double y, double z) const override;

    bool isSpatial() const override;

    void Save(const std::string &fileName, double exposition) const override;
};
