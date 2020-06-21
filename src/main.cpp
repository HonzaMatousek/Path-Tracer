#include <iostream>

#include "body/Scene.h"

int main(int argc, char * argv[]) {
    if(argc != 2) {
        std::cout << "Usage: raytracer <scene_file>" << std::endl;
        return 1;
    }

    Scene scene(argv[1]);
    return 0;
}
