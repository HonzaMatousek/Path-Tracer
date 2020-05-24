#include <iostream>

#include "body/Sphere.h"
#include "camera/Camera.h"

int main() {
    Sphere sphere(Vector3D(10,-1,0), 1);
    Camera camera(Vector3D(0,0,0), Vector3D(1,0,0), Vector3D(0,1,0), 50, 30, 30);

    for(int y = 0; y < 30; y++) {
        for(int x = 0; x < 50; x++) {
            std::cout << (sphere.Intersect(camera.Project(x,y)) ? '#' : ' ' );
        }
        std::cout << std::endl;
    }
    return 0;
}
