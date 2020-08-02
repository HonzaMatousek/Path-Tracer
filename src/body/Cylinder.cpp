//
// Created by honza on 02.08.20.
//

#include "Cylinder.h"

Cylinder::Cylinder(const Vector3D &center, const Vector3D &direction, double radius, double lowCap, double highCap, const Material &material) : Sphere(center + direction * (lowCap + highCap) / 2, sqrt((highCap - lowCap) * (highCap - lowCap) + radius * radius), material), center(center), direction(direction), radius(radius), radiusSquared(radius * radius), lowCap(lowCap), highCap(highCap) {
    this->direction.Normalize();
    worldBasis = Transform::SomeBasisForZ(this->direction).operator*(Transform::Translation(-center.x, -center.y, -center.z));
    circleBasis = worldBasis.Inverse();
}

void Cylinder::Intersect(const Ray &ray, Intersection &intersection) const {
    Ray ray2D(circleBasis.Apply(ray.origin), circleBasis.ApplyWithoutTranslation(ray.direction));
    Vector3D L = ray2D.origin * -1;
    L.z = 0;
    Vector3D dir2D = ray2D.direction;
    dir2D -= Vector3D(0, 0, dir2D.z);
    double tca = L.Dot(dir2D / dir2D.Length());
    // if (tca < 0) return false;
    double d2 = L.Dot(L) - tca * tca;
    if (d2 > radiusSquared) return;
    double thc = std::sqrt(radiusSquared - d2);
    double t0 = (tca - thc)/* * dir2D.Length()*/ / sqrt(1 - ray2D.direction.z * ray2D.direction.z);
    double t1 = (tca + thc)/* * dir2D.Length()*/ / sqrt(1 - ray2D.direction.z * ray2D.direction.z);

    if (t0 > t1) std::swap(t0, t1);

    /*
     *
     * t0 =  / sqrt(x*x + y*y) = 1
     *
     *
     *
     * t0 = / sqrt(x*x + y*y + z*z) = 1
     *
     *
     */

    if (t0 >= 0) {
        Vector3D point = ray2D.Point(t0);
        if(point.z > lowCap && point.z < highCap) {
            point.z = 0;
            intersection.ChooseCloser(Intersection(t0, this, worldBasis.ApplyWithoutTranslation(point)));
            return;
        }
    }

    if (t1 >= 0) {
        Vector3D point = ray2D.Point(t1);
        if(point.z > lowCap && point.z < highCap) {
            point.z = 0;
            intersection.ChooseCloser(Intersection(t1, this, worldBasis.ApplyWithoutTranslation(point)));
        }
    }
}

