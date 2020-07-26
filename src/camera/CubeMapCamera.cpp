//
// Created by honza on 26.07.20.
//

#include "CubeMapCamera.h"

CubeMapCamera::CubeMapCamera(const Vector3D &eye, int width, int height) : eye(eye), width(width), height(height) {}

Ray CubeMapCamera::Project(double w, double h) const {
    if(w >= width / 4 && w < width / 2 && h < height / 3) {
        double wcenter = width * 3.0 / 8.0;
        double hcenter = height * 1.0 / 6.0;
        double wdiff = (w - wcenter) / (width / 8.0);
        double hdiff = (h - hcenter) / (height / 6.0);
        // view up
        return Ray(eye, Vector3D(hdiff, 1, wdiff));
    }
    if(w >= width / 4 && w < width / 2 && h > height / 3 && h < height * 2 / 3) {
        double wcenter = width * 3.0 / 8.0;
        double hcenter = height * 3.0 / 6.0;
        double wdiff = (w - wcenter) / (width / 8.0);
        double hdiff = (hcenter - h) / (height / 6.0);
        // view front
        return Ray(eye, Vector3D(1, hdiff, wdiff));
    }
    if(w >= width / 4 && w < width / 2 && h > height * 2.0 / 3.0) {
        double wcenter = width * 3.0 / 8.0;
        double hcenter = height * 5.0 / 6.0;
        double wdiff = (w - wcenter) / (width / 8.0);
        double hdiff = (hcenter - h) / (height / 6.0);
        // view down
        return Ray(eye, Vector3D(hdiff, -1, wdiff));
    }
    if(w < width / 4 && h > height / 3 && h < height * 2 / 3) {
        double wcenter = width * 1.0 / 8.0;
        double hcenter = height * 3.0 / 6.0;
        double wdiff = (w - wcenter) / (width / 8.0);
        double hdiff = (hcenter - h) / (height / 6.0);
        // view left
        return Ray(eye, Vector3D(wdiff, hdiff, -1));
    }
    if(w > width / 2 && w < width * 3.0 / 4 && h > height / 3 && h < height * 2 / 3) {
        double wcenter = width * 5.0 / 8.0;
        double hcenter = height * 3.0 / 6.0;
        double wdiff = (wcenter - w) / (width / 8.0);
        double hdiff = (hcenter - h) / (height / 6.0);
        // view right
        return Ray(eye, Vector3D(wdiff, hdiff, 1));
    }
    if(w > width * 3.0 / 4 && h > height / 3 && h < height * 2 / 3) {
        double wcenter = width * 7.0 / 8.0;
        double hcenter = height * 3.0 / 6.0;
        double wdiff = (wcenter - w) / (width / 8.0);
        double hdiff = (hcenter - h) / (height / 6.0);
        // view back
        return Ray(eye, Vector3D(-1, hdiff, wdiff));
    }
    return Ray(eye, Vector3D(0,0,0));
}
