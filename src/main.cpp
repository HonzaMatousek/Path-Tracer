#include <iostream>

#include "body/Sphere.h"
#include "body/Scene.h"
#include "camera/Camera.h"

#include "image/ImageJPEG.h"

const int image_width = 800;
const int image_height = 600;

int main() {
    Material greenLight(Color(0,100,0), Color(0,0,0));
    Material mirror(Color(0,0,0), Color(1,1,1));

    Scene scene;
    scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,0), 1, greenLight));
    scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,2), 0.5, mirror));
    Camera camera(Vector3D(0,0,0), Vector3D(1,0,0), Vector3D(0,1,0), image_width, image_height, 30);

    ImageJPEG image(image_width, image_height, 75);

    for(int y = 0; y < image_height; y++) {
        for(int x = 0; x < image_width; x++) {
            auto intersection = scene.Intersect(camera.Project(x,y));
            if(intersection) {
                image.AddPixel(x,y, intersection->material.emissive);
            }
        }
    }

    image.Save("image.jpeg", 255);

    return 0;
}
