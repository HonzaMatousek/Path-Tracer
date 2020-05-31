#include <iostream>
#include <random>

#include "body/Scene.h"
#include "body/Sphere.h"
#include "body/Plane.h"

#include "camera/Camera.h"
#include "image/ImageJPEG.h"
#include "body/Triangle.h"

const int image_width = 960;
const int image_height = 720;
const int iteration_limit = 3;
const int samples = 1000;

int main() {
    Material greenLight(Color(0,100,0), Color(0,0,0));
    Material redLight(Color(100,0,0), Color(0,0,0));
    Material blueLight(Color(0,0,100), Color(0,0,0));
    Material whiteLight(Color(100,100,100), Color(0,0,0));
    Material mirror(Color(0,0,0), Color(1,1,1));
    Material copper(Color(0,0,0), Color(0.9333,0.6196,0.5373));

    Material whiteDiffuse(Color(0,0,0), Color(0.4,0.4,0.4), false);
    Material redDiffuse(Color(0,0,0), Color(0.4,0.0,0.0), false);
    Material greenDiffuse(Color(0,0,0), Color(0.0,0.4,0.0), false);

    Scene scene;
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,0), 1, greenLight));
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,2), 0.5, mirror));
    //scene.AddBody(std::make_unique<Plane>(Vector3D(10,-1,2), Vector3D(0,0,-1), mirror));

    std::random_device r;
    std::uniform_real_distribution d1(-2.0,2.0);
    std::uniform_real_distribution d2(0.0,1.0);

    /*for(int i = 0; i < 20; i++) {
        scene.AddBody(std::make_unique<Sphere>(Vector3D(d1(r) + 10,d1(r),d1(r)*2), d2(r), Material(Color(d2(r),d2(r),d2(r)), Color(d2(r),d2(r),d2(r)))));
    }*/

    //scene.AddBody(std::make_unique<Triangle>(Vector3D(10,-1,1), Vector3D(10,1,0), Vector3D(10,-1,-1), greenLight));

    // protejsi stena
    scene.AddBody(std::make_unique<Triangle>(Vector3D(5,-1,1), Vector3D(5,1,-1), Vector3D(5,-1,-1), whiteDiffuse));
    scene.AddBody(std::make_unique<Triangle>(Vector3D(5,-1,1), Vector3D(5,1,1), Vector3D(5,1,-1), whiteDiffuse));

    // leva
    scene.AddBody(std::make_unique<Triangle>(Vector3D(5,-1,1), Vector3D(0,-1,1), Vector3D(0,1,1), redDiffuse));
    scene.AddBody(std::make_unique<Triangle>(Vector3D(5,-1,1), Vector3D(0,1,1), Vector3D(5,1,1), redDiffuse));

    // prava
    scene.AddBody(std::make_unique<Triangle>(Vector3D(5,-1,-1), Vector3D(0,1,-1), Vector3D(0,-1,-1), greenDiffuse));
    scene.AddBody(std::make_unique<Triangle>(Vector3D(5,-1,-1), Vector3D(5,1,-1), Vector3D(0,1,-1), greenDiffuse));

    // podlaha
    scene.AddBody(std::make_unique<Plane>(Vector3D(5,-1,-1), Vector3D(0,1,0), whiteDiffuse));

    // strop
    scene.AddBody(std::make_unique<Plane>(Vector3D(5,1,-1), Vector3D(0,-1,0), whiteDiffuse));

    // svetlo
    scene.AddBody(std::make_unique<Triangle>(Vector3D(4.5,0.9995,-0.875), Vector3D(4.5,0.9995,0.875), Vector3D(4,0.9995,-0.875), whiteLight));
    scene.AddBody(std::make_unique<Triangle>(Vector3D(4.0,0.9995,0.875), Vector3D(4,0.9995,-0.875), Vector3D(4.5,0.9995,0.875), whiteLight));

    // koule
    scene.AddBody(std::make_unique<Sphere>(Vector3D(3,-0.75,-0.25), 0.25, whiteDiffuse));

    scene.AddBody(std::make_unique<Sphere>(Vector3D(3.5,-0.80,0.25), 0.20, copper));

    Camera camera(Vector3D(0,-0,0), Vector3D(1,0,0), Vector3D(0,1,0), image_width, image_height, 60);

    ImageJPEG image(image_width, image_height, 75);

    for(int sample = 0; sample < samples; sample++) {
        for (int y = 0; y < image_height; y++) {
            for (int x = 0; x < image_width; x++) {
                Ray ray = camera.Project(x, y);
                Color albedoMultiplier(1, 1, 1);
                for (int i = 0; i < iteration_limit; i++) {
                    auto intersection = scene.Intersect(ray);
                    if (intersection) {
                        image.AddPixel(x, y, intersection->material.emissive * albedoMultiplier);
                        albedoMultiplier *= intersection->material.albedo;
                        if (albedoMultiplier == Color()) {
                            break;
                        }
                        double power = 1;
                        ray = intersection->Reflect(ray, power);
                        albedoMultiplier *= power;
                    } else {
                        break;
                    }
                }
            }
        }
        std::cout << sample << "/" << samples << std::endl;
    }

    image.Save("image.jpeg", 100.0 / samples);

    return 0;
}
