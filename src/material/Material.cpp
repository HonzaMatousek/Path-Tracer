#include "Material.h"

Material Material::operator+(const Material & rhs) const {
    return Material(emissive + rhs.emissive, (albedo + rhs.albedo) / 2, reflective, (roughness + rhs.roughness) / 2);
}

Material & Material::operator+=(const Material & rhs) {
    emissive += rhs.emissive;
    albedo += rhs.albedo;
    albedo /= 2;
    roughness += rhs.roughness;
    roughness /= 2;
    return *this;
}

Material Material::operator-(const Material & rhs) const {
    return Material(emissive + rhs.emissive, (albedo - rhs.albedo) / 2, reflective, (roughness + rhs.roughness) / 2);
}

Material & Material::operator-=(const Material & rhs) {
    emissive -= rhs.emissive;
    albedo -= rhs.albedo;
    albedo /= 2;
    roughness += rhs.roughness;
    roughness /= 2;
    return *this;
}

Material Material::operator*(double scale) const {
    return Material(emissive * scale, albedo * scale, reflective, roughness);
}

Material &Material::operator*=(double scale) {
    emissive *= scale;
    albedo *= scale;
    return *this;
}
