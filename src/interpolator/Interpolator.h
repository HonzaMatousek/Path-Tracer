#pragma once

#include <memory>
#include "../math/Vector3D.h"
#include "../material/Material.h"
#include "../image/Image.h"
#include "../material/TexturedMaterial.h"
#include "../math/Transform.h"

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

class NormalizeInterpolator : public Interpolator<Transform> {
public:
    [[ nodiscard ]]
    Transform Interpolate(const Vector3D & coordinates) const override {
        return Transform::SomeBasisForZ(Vector3D(coordinates).Normalize());
    }
};

class PlaneNormalInterpolator : public FlatInterpolator<Transform> {
public:
    explicit PlaneNormalInterpolator(const Vector3D & originalNormal) : FlatInterpolator(Transform::SomeBasisForZ(originalNormal)) {}
    explicit PlaneNormalInterpolator(const Vector3D & a, const Vector3D & b, const Vector3D & c) : FlatInterpolator(Transform(a, b, c)) {}
};

class PlaneNormalRoundingInterpolator : public Interpolator<Transform> {
    std::unique_ptr<Interpolator<Transform>> base;
    std::unique_ptr<TriangleInterpolator<Vector3D>> triangleInterpolator;
public:
    PlaneNormalRoundingInterpolator(std::unique_ptr<Interpolator<Transform>> base, const Vector3D & a, const Vector3D & b, const Vector3D & c) : base(std::move(base)), triangleInterpolator(std::make_unique<TriangleInterpolator<Vector3D>>(a, b, c)) {}

    [[ nodiscard ]]
    Transform Interpolate(const Vector3D & coordinates) const override {
        auto tr = base->Interpolate(coordinates);
        auto n = triangleInterpolator->Interpolate(coordinates).Normalize();
        tr.ReplaceThird(n);
        return tr;
    }
};

class PassThroughInterpolator : public Interpolator<Vector3D> {
public:
    [[ nodiscard ]]
    Vector3D Interpolate(const Vector3D & coordinates) const override {
        return coordinates;
    }
};

class SpherePolarInterpolator : public Interpolator<Vector3D> {
    std::unique_ptr<Interpolator<Vector3D>> base;
public:
    SpherePolarInterpolator(std::unique_ptr<Interpolator<Vector3D>> && base = nullptr) : base(std::move(base)) {}

    [[ nodiscard ]]
    Vector3D Interpolate(const Vector3D & coordinates) const override {
        Vector3D coord = base ? base->Interpolate(coordinates) : coordinates;
        double bg_y = acos(coord.y / coord.Length()) / M_PI;
        double bg_x = atan2(coord.z, coord.x) / 2 / M_PI + 0.5;
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

class LegacyTextureInterpolator : public Interpolator<Material> {
    std::unique_ptr<Interpolator<Vector3D>> base;
    std::shared_ptr<Image> emissionTexture;
    std::shared_ptr<Image> albedoTexture;
    bool reflective;
    double roughness;
public:
    LegacyTextureInterpolator(std::unique_ptr<Interpolator<Vector3D>> && base, const std::shared_ptr<Image> & emissionTexture, const std::shared_ptr<Image> & albedoTexture, bool reflective, double roughness) : base(std::move(base)), emissionTexture(emissionTexture), albedoTexture(albedoTexture), reflective(reflective), roughness(roughness) {}

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

class TextureInterpolator : public Interpolator<Material> {
    std::unique_ptr<Interpolator<Vector3D>> base;
    const TexturedMaterial * material;
public:
    TextureInterpolator(std::unique_ptr<Interpolator<Vector3D>> && base, const TexturedMaterial * material) : base(std::move(base)), material(material) {}

    [[ nodiscard ]]
    Material Interpolate(const Vector3D & coordinates) const override {
        Vector3D tex_coords(base->Interpolate(coordinates));
        return Material(
                material->emissiveTexture ? material->emissiveTexture->GetPixel(tex_coords.x, tex_coords.y, tex_coords.z) : material->base.emissive,
                material->albedoTexture ? material->albedoTexture->GetPixel(tex_coords.x, tex_coords.y, tex_coords.z) : material->base.albedo,
                material->base.reflective,
                material->base.roughness,
                material->base.refractiveIndex,
                material->base.opacity,
                material->normalTexture ? material->normalTexture->GetPixel(tex_coords.x, tex_coords.y, tex_coords.z).ToNormal() : material->base.normal
        );
    }
};
