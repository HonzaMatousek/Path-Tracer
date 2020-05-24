#include <iostream>

#include "body/Sphere.h"
#include "body/Scene.h"
#include "camera/Camera.h"

int main() {
    Scene scene;
    scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,0), 1));
    scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,2), 0.5));
    Camera camera(Vector3D(0,0,0), Vector3D(1,0,0), Vector3D(0,1,0), 50, 30, 30);

    for(int y = 0; y < 30; y++) {
        for(int x = 0; x < 50; x++) {
            std::cout << (scene.Intersect(camera.Project(x,y)) ? '#' : ' ' );
        }
        std::cout << std::endl;
    }
    return 0;
}
