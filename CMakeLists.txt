cmake_minimum_required(VERSION 3.15)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer src/main.cpp src/body/Sphere.cpp src/body/Sphere.h src/body/Body.cpp src/body/Body.h src/math/Vector3D.cpp src/math/Vector3D.h src/math/Ray.cpp src/math/Ray.h src/math/Intersection.cpp src/math/Intersection.h src/camera/Camera.cpp src/camera/Camera.h src/body/Scene.cpp src/body/Scene.h src/image/Image.cpp src/image/Image.h src/image/ImageJPEG.cpp src/image/ImageJPEG.h src/material/Color.cpp src/material/Color.h src/material/Material.cpp src/material/Material.h src/body/Plane.cpp src/body/Plane.h)

target_link_libraries(raytracer libjpeg.so)