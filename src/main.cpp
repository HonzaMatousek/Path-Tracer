#include <iostream>

#include "body/Scene.h"

int main(int argc, char * argv[]) {
    if(argc != 2) {
        std::cout << "Usage: raytracer <scene_file>" << std::endl;
        return 1;
    }

    try {
        Scene scene(argv[1]);
    }
    catch(std::exception & e) {
        std::cerr << "Program failed:" << std::endl << e.what() << std::endl;
    }
    return 0;
}
