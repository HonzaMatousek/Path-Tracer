#include <iostream>
#include <random>
#include <mutex>
#include <thread>

#include "body/Scene.h"
#include "body/Sphere.h"
#include "body/Plane.h"

#include "camera/Camera.h"
#include "image/ImageJPEG.h"
#include "body/Triangle.h"
#include "material/BlackBodyRadiation.h"
#include "model/ModelOBJ.h"
#include "image/ImageTGA.h"

const int image_width = 1920;// 1920;
const int image_height = 1080; //1080;
const int iteration_limit = 5;
const int samples = 10;

class LineEmployer {
    int freeLine = 0;
    std::mutex freeLineMutex;
public:
    int getLine() {
        std::lock_guard guard(freeLineMutex);
        return ++freeLine;
    }
};

void render(std::mt19937& generator, const Camera & camera, const Scene & scene, Image & image, LineEmployer & lineEmployer) {
    std::uniform_real_distribution<double> d(-0.5,0.5);
    for (int y = lineEmployer.getLine(); y < image_height; y = lineEmployer.getLine()) {
        for (int x = 0; x < image_width; x++) {
            double cam_x = d(generator);
            double cam_y = d(generator);
            Ray ray = camera.Project(x + d(generator), y + d(generator));
            //Ray ray = camera.Project(x, y);
            Color albedoMultiplier(1, 1, 1);
            albedoMultiplier *= 1 - std::abs(cam_x * cam_y);
            for (int i = 0; i < iteration_limit; i++) {
                Intersection intersection(&scene, ray);
                scene.Intersect(ray, intersection);
                auto material = intersection.GetMaterial();
                image.AddPixel(x, y, material.emissive * albedoMultiplier);
                albedoMultiplier *= material.albedo;
                if (albedoMultiplier == Color()) {
                    break;
                }
                double power = 1;
                ray = intersection.Reflect(ray, power, generator);
                albedoMultiplier *= power;
            }
        }
    }
}

int main() {
    Material greenLight(Color(0,100,0), Color(0,0,0));
    Material redLight(Color(100,0,0), Color(0,0,0));
    Material blueLight(Color(0,0,100), Color(0,0,0));
    Material whiteLight(Color(100,100,100), Color(0,0,0));
    Material mirror(Color(0,0,0), Color(1,1,1));
    Material copper(Color(0,0,0), Color(0.9333,0.6196,0.5373));

    Material whiteDiffuse(Color(0,0,0), Color(0.95,0.95,0.95), false);
    Material redDiffuse(Color(0,0,0), Color(0.95,0.0,0.0), false);
    Material orangeDiffuse(Color(0,0,0), Color(0.95,0.95,0.0), false);
    Material greenDiffuse(Color(0,0,0), Color(0.0,0.95,0.0), false);
    Material cyanDiffuse(Color(0,0,0), Color(0.0,0.95,0.95), false);
    Material beigeDiffuse(Color(0,0,0), Color(0.95,0.95,0.7), false);

    Material sun(BlackBodyRadiation(5778), Color(0,0,0));
    Material moltenCopper(BlackBodyRadiation(1085 + 273.16), Color(0.9333,0.6196,0.5373));
    Material moltenIron(BlackBodyRadiation(1812), Color(0.8863,0.8745,0.8235));
    Material moltenTungsten(BlackBodyRadiation(3300), Color(236, 213, 193) / 255);

    Scene scene;
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,0), 1, greenLight));
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(10,-1,2), 0.5, mirror));
    //scene.AddBody(std::make_unique<Plane>(Vector3D(10,-1,2), Vector3D(0,0,-1), mirror));

    std::random_device r;
    std::mt19937 generator(r());
    std::uniform_real_distribution d1(-2.0,2.0);
    std::uniform_real_distribution d2(0.0,1.0);

    /*for(int i = 0; i < 2000; i++) {
        scene.AddBody(std::make_unique<Sphere>(Vector3D(d1(r) + 10,d1(r),d1(r)*2), d2(r), Material(Color(d2(r),d2(r),d2(r)), Color(d2(r),d2(r),d2(r)))));
    }*/

    //ModelOBJ::Import("../03.obj", scene);
    ModelOBJ::Import("../bunny.obj", scene);
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(4,4,4), 2, whiteLight));

    //scene.AddBody(std::make_unique<Triangle>(Vector3D(10,-1,1), Vector3D(10,1,0), Vector3D(10,-1,-1), greenLight));

    Vector3D a(-1.5,0-0.170126,-1);
    Vector3D b(-1.5,0-0.170126,10);
    Vector3D c(1.5,0-0.170126,10);
    Vector3D d(1.5,0-0.170126,-1);
    Vector3D e(-1.5,2-0.170126,-1);
    Vector3D f(-1.5,2-0.170126,10);
    Vector3D g(1.5,2-0.170126,10);
    Vector3D h(1.5,2-0.170126,-1);

    Vector3D le(-0.5, 2-0.171, -0.5);
    Vector3D lf(-0.5, 2-0.171, 0.5);
    Vector3D lg(0.5, 2-0.171, 0.5);
    Vector3D lh(0.5, 2-0.171, -0.5);

    // protejsi stena
    /*scene.AddBody(std::make_unique<Triangle>(a * 2, d * 2, e * 2, whiteDiffuse));
    scene.AddBody(std::make_unique<Triangle>(d * 2, h * 2, e * 2, whiteDiffuse));

    // leva
    scene.AddBody(std::make_unique<Triangle>(c * 2, g * 2, d * 2, cyanDiffuse));
    scene.AddBody(std::make_unique<Triangle>(d * 2, g * 2, h * 2, cyanDiffuse));

    // prava
    scene.AddBody(std::make_unique<Triangle>(a * 2, e * 2, b * 2, orangeDiffuse));
    scene.AddBody(std::make_unique<Triangle>(b * 2, e * 2, f * 2, orangeDiffuse));

    // strop
    scene.AddBody(std::make_unique<Triangle>(e * 2, g * 2, f * 2, whiteDiffuse));
    scene.AddBody(std::make_unique<Triangle>(e * 2, h * 2, g * 2, whiteDiffuse));

    // podlaha
    scene.AddBody(std::make_unique<Triangle>(a * 2, b * 2, c * 2, whiteDiffuse));
    scene.AddBody(std::make_unique<Triangle>(a * 2, c * 2, d * 2, whiteDiffuse));

    // svetlo
    scene.AddBody(std::make_unique<Triangle>(le * 2, lg * 2, lf * 2, whiteLight));
    scene.AddBody(std::make_unique<Triangle>(le * 2, lh * 2, lg * 2, whiteLight));*/

    // prava
    //scene.AddBody(std::make_unique<Triangle>(Vector3D(5,-1,-1), Vector3D(0,1,-1), Vector3D(0,-1,-1), greenDiffuse));
    //scene.AddBody(std::make_unique<Triangle>(Vector3D(5,-1,-1), Vector3D(5,1,-1), Vector3D(0,1,-1), greenDiffuse));
    //scene.AddBody(std::make_unique<Plane>(Vector3D(5,-1,-1), Vector3D(0,0,1), greenDiffuse));

    // podlaha
    //scene.AddBody(std::make_unique<Plane>(Vector3D(5,-1,-1), Vector3D(0,1,0), whiteDiffuse));

    // strop
    //scene.AddBody(std::make_unique<Plane>(Vector3D(5,1,-1), Vector3D(0,-1,0), whiteDiffuse));

    // svetlo
    //scene.AddBody(std::make_unique<Triangle>(Vector3D(4.5,0.9995,-0.875), Vector3D(4.5,0.9995,0.875), Vector3D(4,0.9995,-0.875), whiteLight));
    //scene.AddBody(std::make_unique<Triangle>(Vector3D(4.0,0.9995,0.875), Vector3D(4,0.9995,-0.875), Vector3D(4.5,0.9995,0.875), whiteLight));

    // koule
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(4.35,-0.55,-0.25), 0.45, beigeDiffuse));

    //scene.AddBody(std::make_unique<Sphere>(Vector3D(14.35,-0.55,-0.25), 0.45, beigeDiffuse));

    //scene.AddBody(std::make_unique<Sphere>(Vector3D(24.35,-0.55,-0.25), 0.45, beigeDiffuse));

    //scene.AddBody(std::make_unique<Sphere>(Vector3D(34.35,-0.55,-0.25), 0.45, beigeDiffuse));

    //scene.AddBody(std::make_unique<Sphere>(Vector3D(3.625,-0.80,0.25), 0.20, copper));

    //scene.AddBody(std::make_unique<Sphere>(Vector3D(4,0,-1), 1, moltenIron));
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(4,0,1), 0.8, moltenCopper));
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(3,-0.1,0), 0.2, moltenTungsten)); // in LightBulb
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(1,1,0), 0.45, beigeDiffuse));
    //scene.AddBody(std::make_unique<Sphere>(Vector3D(-1,1,0), 0.45, copper));
    /*std::shared_ptr<Image> texture(new ImageJPEG("../earth.jpg", 1.0, 75));
    auto sphere = std::make_unique<Sphere>(Vector3D(0,1,0), 1, copper);
    sphere->SetMaterialInterpolator(
            std::make_unique<TextureInterpolator>(std::make_unique<SpherePolarInterpolator>(),
                                                  nullptr,
                                                  texture,
                                                  false,
                                                  0.0
            )
            );
    scene.AddBody(std::move(sphere));*/

    //Camera camera(Vector3D(-3,1,4), Vector3D(0.75,0,-1), Vector3D(0,1,0), image_width, image_height, 90);
    Camera camera(Vector3D(0,1,4), Vector3D(0,0,-1), Vector3D(0,1,0), image_width, image_height, 90);

    ImageJPEG image(image_width, image_height, 75);
    std::shared_ptr<Image> bg(new ImageJPEG("../sky.jpg", 0.1, 75));
    //scene.SetMaterialInterpolator(std::make_unique<FlatInterpolator<Material>>(Material(Color(1,0,0), Color(), false)));
    scene.SetMaterialInterpolator(
            std::make_unique<TextureInterpolator>(std::make_unique<SpherePolarInterpolator>(),
                                                  bg,
                                                  nullptr,
                                                  false,
                                                  0.0
            )
    );

    auto start = std::chrono::steady_clock::now();
    std::cout << "Compiling KD tree..." << std::endl;
    scene.Compile();
    std::cout << "KD tree compiled." << std::endl;
    std::cout << "Rendering. Using " << std::thread::hardware_concurrency() << " threads." << std::endl;
    std::vector<std::mt19937> generators;
    generators.reserve(std::thread::hardware_concurrency());
    for(int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        generators.emplace_back(std::mt19937(r()));
    }
    for(int sample = 0; sample < samples; sample++) {
        std::vector<std::thread> threads;
        threads.reserve(std::thread::hardware_concurrency());
        LineEmployer lineEmployer;
        for(int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            threads.emplace_back(std::thread(render, std::ref(generator), std::ref(camera), std::ref(scene), std::ref(image), std::ref(lineEmployer)));
        }
        for(auto & thread : threads) {
            thread.join();
        }
        std::cout << (sample + 1) << "/" << samples << std::endl;
        /*if(sample % 100 == 99) {
            image.Save("image7.tga", 0.1 / (sample + 1));
        }*/
    }

    image.Save("stream04_d.jpeg", 0.1 / samples);
    auto stop = std::chrono::steady_clock::now();
    std::cout << "Finished, took " << ((stop - start).count() / 1e9) <<  " s." << std::endl;

    return 0;
}
