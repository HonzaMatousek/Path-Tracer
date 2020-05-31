#include <iostream>
#include <random>

#include "body/Scene.h"
#include "body/Sphere.h"
#include "body/Plane.h"

#include "camera/Camera.h"
#include "image/ImageJPEG.h"

const int image_width = 960;
const int image_height = 720;
const int iteration_limit = 10;

int main() {
    Material greenLight(Color(0,100,0), Color(0,0,0));
    Material mirror(Color(0,0,0.1), Color(1,1,1));

    Scene scene;
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,0), 1, greenLight));
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,2), 0.5, mirror));
    //scene.AddBody(std::make_unique<Plane>(Vector3D(10,-1,2), Vector3D(0,0,-1), mirror));

    std::random_device r;
    std::uniform_real_distribution d1(-2.0,2.0);
    std::uniform_real_distribution d2(0.0,1.0);

    for(int i = 0; i < 20; i++) {
        scene.AddBody(std::make_unique<Sphere>(Vector3D(d1(r) + 10,d1(r),d1(r)*2), d2(r), Material(Color(d2(r),d2(r),d2(r)), Color(d2(r),d2(r),d2(r)))));
    }

    Camera camera(Vector3D(0,0,0), Vector3D(1,0,0), Vector3D(0,1,0), image_width, image_height, 60);

    ImageJPEG image(image_width, image_height, 75);

    for(int y = 0; y < image_height; y++) {
        for(int x = 0; x < image_width; x++) {
            Ray ray = camera.Project(x, y);
            Color albedoMultiplier(1,1,1);
            for(int i = 0; i < iteration_limit; i++) {
                auto intersection = scene.Intersect(ray);
                if (intersection) {
                    image.AddPixel(x, y, intersection->material.emissive * albedoMultiplier);
                    albedoMultiplier *= intersection->material.albedo;
                    if(albedoMultiplier == Color()) {
                        break;
                    }
                    ray = intersection->Reflect(ray);
                }
                else {
                    break;
                }
            }
        }
    }

    image.Save("image.jpeg", 255);

    return 0;
}
