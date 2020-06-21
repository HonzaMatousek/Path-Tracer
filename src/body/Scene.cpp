//
// Created by honza on 24.05.20.
//

#include "Scene.h"
#include "../model/ModelOBJ.h"
#include "Sphere.h"
#include "../camera/Camera.h"
#include "../image/ImageJPEG.h"
#include "Triangle.h"
#include "../math/Transform.h"
#include <fstream>
#include <sstream>
#include <map>
#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <stack>

class LineEmployer {
    int freeLine = 0;
    std::mutex freeLineMutex;
public:
    int getLine() {
        std::lock_guard guard(freeLineMutex);
        return ++freeLine;
    }
};

inline const int iteration_limit = 5;

void Scene::RenderCore(std::mt19937& generator, LineEmployer & lineEmployer) const {
    std::uniform_real_distribution<double> d(-0.5,0.5);
    for (int y = lineEmployer.getLine(); y < image_height; y = lineEmployer.getLine()) {
        for (int x = 0; x < image_width; x++) {
            double cam_x = d(generator);
            double cam_y = d(generator);
            Ray ray = camera->Project(x + d(generator), y + d(generator));
            //Ray ray = camera.Project(x, y);
            Color albedoMultiplier(1, 1, 1);
            albedoMultiplier *= 1 - std::abs(cam_x * cam_y);
            for (int i = 0; i < iteration_limit; i++) {
                Intersection intersection(this, ray);
                Intersect(ray, intersection);
                auto material = intersection.GetMaterial();
                image->AddPixel(x, y, material.emissive * albedoMultiplier);
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

void Scene::AddBody(std::unique_ptr<Body> body) {
    bodies.emplace_back(std::move(body));
}

void Scene::Intersect(const Ray & ray, Intersection & intersection) const {
    if(kdTree) {
        kdTree->Intersect(ray, intersection);
    }
    else for(const auto & body : bodies) {
        body->Intersect(ray, intersection);
    }
}

void Scene::Compile() {
    std::vector<Body*> treeBodies;
    Vector3D l(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    Vector3D u(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
    for(const auto & body : bodies) {
        treeBodies.push_back(body.get());
        l = l.Min(body->lowerCorner);
        u = u.Max(body->upperCorner);
    }
    kdTree = std::make_unique<KDTree>(treeBodies, l, u);
}

Scene::Scene() : Body(std::make_unique<FlatInterpolator<Material>>(Material())) {}

Scene::Scene(std::string fileName) : Body(std::make_unique<FlatInterpolator<Material>>(Material())) {
    //SetMaterialInterpolator(std::make_unique<FlatInterpolator<Material>>(Material(Color(0,0,0), Color(), false)));
    std::ifstream file(fileName);
    std::string line;
    std::map<std::string, Material> materialLib {
            {"whiteDiffuse", Material(Color(0,0,0), Color(0.95,0.95,0.95), false)},
            {"whiteLight", Material(Color(100,100,100), Color(0,0,0))},
            {"whiteRough", Material(Color(0,0,0), Color(0.95,0.92,0.90), true, 0.0002)},
    };
    Material * current_material = &materialLib["whiteDiffuse"];
    std::stack<Transform> transforms;
    transforms.push(Transform::Scale(1));
    while(std::getline(file, line)) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(command == "#") {
            // skip comment
        }
        else if(command == "move") {
            double x, y, z;
            lineStream >> x >> y >> z;
            transforms.push(transforms.top() * Transform::Translation(x, y, z));
        }
        else if(command == "rotate_y") {
            double deg;
            lineStream >> deg;
            transforms.push(transforms.top() * Transform::RotateY(deg / 180.0 * M_PI));
        }
        else if(command == "pop_transform") {
            if(transforms.size() == 1) {
                throw std::runtime_error("Popping poop, GTFO!");
            }
            transforms.pop();
        }
        else if(command == "model") {
            std::string modelPath;
            lineStream >> modelPath;
            ModelOBJ::Import(modelPath, transforms.top(), *this);
        }
        else if(command == "sphere") {
            double x, y, z, radius;
            lineStream >> x >> y >> z >> radius;
            AddBody(std::make_unique<Sphere>(transforms.top().Apply(Vector3D(x, y, z)), radius, *current_material));
        }
        else if(command == "triangle") {
            double ax, ay, az, bx, by, bz, cx, cy, cz;
            lineStream >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz;
            AddBody(std::make_unique<Triangle>(transforms.top().Apply(Vector3D(ax, ay, az)), transforms.top().Apply(Vector3D(bx, by, bz)), transforms.top().Apply(Vector3D(cx, cy, cz)), *current_material));
        }
        else if(command == "current_material") {
            std::string newCurrentMaterialName;
            lineStream >> newCurrentMaterialName;
            auto it = materialLib.find(newCurrentMaterialName);
            if(it == materialLib.end()) {
                throw std::runtime_error("Material not found, GTFO!");
            }
            current_material = &(it->second);
        }
        else if(command == "background_texture") {
            std::string texturePath;
            lineStream >> texturePath;
            std::shared_ptr<Image> bg(new ImageJPEG(texturePath, 1, 95));
            SetMaterialInterpolator(
                std::make_unique<TextureInterpolator>(std::make_unique<SpherePolarInterpolator>(),
                  bg,
                  nullptr,
                  false,
                  0.0
                )
            );
        }
        else if(command == "camera") {
            double eye_x, eye_y, eye_z, dir_x, dir_y, dir_z, up_x, up_y, up_z, fov;
            lineStream >> eye_x >> eye_y >> eye_z >> dir_x >> dir_y >> dir_z >> up_x >> up_y >> up_z >> image_width >> image_height >> fov;
            camera = std::make_unique<Camera>(transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)),
                                              transforms.top().ApplyWithoutTranslation(Vector3D(dir_x, dir_y, dir_z)),
                                              transforms.top().ApplyWithoutTranslation(Vector3D(up_x, up_y, up_z)), image_width, image_height, fov);
        }
        else if(command == "output") {
            std::string outputFileName;
            double exposition;
            lineStream >> outputFileName >> samples >> exposition;
            image = std::make_unique<ImageJPEG>(image_width, image_height, 95);
            Render();
            image->Save(outputFileName, exposition / samples);
        }
    }
}

void Scene::Render() {
    auto start = std::chrono::steady_clock::now();
    std::cout << "Compiling KD tree..." << std::endl;
    Compile();
    std::cout << "KD tree compiled." << std::endl;
    std::cout << "Rendering. Using " << std::thread::hardware_concurrency() << " threads." << std::endl;
    std::random_device r;
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
            threads.emplace_back(std::thread(&Scene::RenderCore, this, std::ref(generators[i]), std::ref(lineEmployer)));
        }
        for(auto & thread : threads) {
            thread.join();
        }
        std::cout << (sample + 1) << "/" << samples << std::endl;
    }
    auto stop = std::chrono::steady_clock::now();
    std::cout << "Finished, took " << ((stop - start).count() / 1e9) <<  " s." << std::endl;
}

Scene::~Scene() = default;
