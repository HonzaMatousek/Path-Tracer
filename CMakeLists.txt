cmake_minimum_required(VERSION 3.15)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O3")
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

add_executable(raytracer src/main.cpp src/body/Sphere.cpp src/body/Sphere.h src/body/Body.cpp src/body/Body.h src/math/Vector3D.cpp src/math/Vector3D.h src/math/Ray.cpp src/math/Ray.h src/math/Intersection.cpp src/math/Intersection.h src/camera/Camera.cpp src/camera/Camera.h src/body/Scene.cpp src/body/Scene.h src/image/Image.cpp src/image/Image.h src/image/ImageJPEG.cpp src/image/ImageJPEG.h src/material/Color.cpp src/material/Color.h src/material/Material.cpp src/material/Material.h src/body/Plane.cpp src/body/Plane.h src/body/Triangle.cpp src/body/Triangle.h src/material/BlackBodyRadiation.cpp src/material/BlackBodyRadiation.h src/body/KDTree.cpp src/body/KDTree.h src/model/Model.cpp src/model/Model.h src/model/ModelOBJ.cpp src/model/ModelOBJ.h src/image/ImageTGA.cpp src/interpolator/Interpolator.cpp src/interpolator/Interpolator.h src/math/Transform.cpp src/math/Transform.h src/material/TexturedMaterial.cpp src/material/TexturedMaterial.h src/shader/Wood.cpp src/shader/Marble.h src/shader/Wood.cpp src/shader/Marble.cpp src/shader/Voronoi.cpp src/shader/Voronoi.h src/math/Util.h src/material/Environment.h src/camera/PerspectiveCamera.cpp src/camera/PerspectiveCamera.h src/camera/OrthogonalCamera.cpp src/camera/OrthogonalCamera.h src/camera/SphericalCamera.cpp src/camera/SphericalCamera.h src/camera/CubeMapCamera.cpp src/camera/CubeMapCamera.h src/effect/Effect.cpp src/effect/Effect.h src/effect/DepthOfFieldEffect.cpp src/effect/DepthOfFieldEffect.h src/body/SphereEnd.cpp src/body/SphereEnd.h src/body/Cylinder.cpp src/body/Cylinder.h)

target_link_libraries(raytracer libjpeg.so libpthread.so)