#pragma once

#include <memory>
#include "../math/Vector3D.h"
#include "../material/Material.h"
#include "../image/Image.h"

template <typename T>
class Interpolator {
public:
    [[ nodiscard ]]
    virtual T Interpolate(const Vector3D & coordinates) const = 0;
};

template <typename T>
class TriangleInterpolator : public Interpolator<T> {
    T a;
    T ab;
    T ac;
public:
    TriangleInterpolator(const T & a, const T & b, const T & c) : a(a), ab(b - a), ac(c - a) {}

    [[ nodiscard ]]
    T Interpolate(const Vector3D & coordinates) const override {
        return a + ab * coordinates.x + ac * coordinates.y;
    }
};

template <typename T>
class FlatInterpolator : public Interpolator<T> {
    T value;
public:
    FlatInterpolator(const T & value) : value(value) {}

    [[ nodiscard ]]
    T Interpolate(const Vector3D & coordinates) const override {
        return value;
    }
};

class NormalizeInterpolator : public Interpolator<Vector3D> {
public:
    [[ nodiscard ]]
    Vector3D Interpolate(const Vector3D & coordinates) const override {
        return Vector3D(coordinates).Normalize();
    }
};

class SpherePolarInterpolator : public Interpolator<Vector3D> {
    std::unique_ptr<Interpolator<Vector3D>> base;
public:
    SpherePolarInterpolator(std::unique_ptr<Interpolator<Vector3D>> && base = nullptr) : base(std::move(base)) {}

    [[ nodiscard ]]
    Vector3D Interpolate(const Vector3D & coordinates) const override {
        Vector3D coord = base ? base->Interpolate(coordinates) : coordinates;
        double bg_y = acos(coordinates.y / coordinates.Length()) / M_PI;
        double bg_x = atan2(coordinates.z, coordinates.x) / 2 / M_PI + 0.5;
        return Vector3D(bg_x, bg_y, 0);
    }
};

class NormalDebugInterpolator : public Interpolator<Material> {
    std::unique_ptr<Interpolator<Vector3D>> base;
public:
    NormalDebugInterpolator(std::unique_ptr<Interpolator<Vector3D>> && base) : base(std::move(base)) {}

    [[ nodiscard ]]
    Material Interpolate(const Vector3D & coordinates) const override {
        Vector3D normal(base->Interpolate(coordinates));
        return Material(Color(
            normal.x / 2 + 0.5,
            normal.y / 2 + 0.5,
            normal.z / 2 + 0.5
        ), Color(), false);
    }
};

class TextureInterpolator : public Interpolator<Material> {
    std::unique_ptr<Interpolator<Vector3D>> base;
    std::shared_ptr<Image> emissionTexture;
    std::shared_ptr<Image> albedoTexture;
    bool reflective;
    double roughness;
public:
    TextureInterpolator(std::unique_ptr<Interpolator<Vector3D>> && base, const std::shared_ptr<Image> & emissionTexture, const std::shared_ptr<Image> & albedoTexture, bool reflective, double roughness) : base(std::move(base)), emissionTexture(emissionTexture), albedoTexture(albedoTexture), reflective(reflective), roughness(roughness) {}

    [[ nodiscard ]]
    Material Interpolate(const Vector3D & coordinates) const override {
        Vector3D tex_coords(base->Interpolate(coordinates));
        return Material(
            emissionTexture ? emissionTexture->GetPixel(tex_coords.x, tex_coords.y) : Color(),
            albedoTexture ? albedoTexture->GetPixel(tex_coords.x, tex_coords.y) : Color(),
            reflective
        );
    }
};
