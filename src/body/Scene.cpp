//
// Created by honza on 24.05.20.
//

#include "Scene.h"
#include "../model/ModelOBJ.h"
#include "Sphere.h"
#include "../camera/Camera.h"
#include "../image/ImageJPEG.h"
#include "../effect/Effect.h"
#include "Triangle.h"
#include "../math/Transform.h"
#include "../shader/Wood.h"
#include "../shader/Marble.h"
#include "../shader/Voronoi.h"
#include "../camera/PerspectiveCamera.h"
#include "../camera/OrthogonalCamera.h"
#include "../camera/SphericalCamera.h"
#include "../camera/CubeMapCamera.h"
#include "../effect/DepthOfFieldEffect.h"
#include "SphereEnd.h"
#include "Cylinder.h"
#include "PlaneCircle.h"
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
        return freeLine++;
    }
};

inline const int iteration_limit = 5;

void Scene::RenderCore(std::mt19937& generator, LineEmployer & lineEmployer) const {
    std::uniform_real_distribution<double> d(-0.5,0.5);
    std::uniform_real_distribution<double> frequencyDistribution(frequencyR / 1.5, frequencyB * 1.5);
    for (int y = lineEmployer.getLine(); y < image_height; y = lineEmployer.getLine()) {
        for (int x = 0; x < image_width; x++) {
            double cam_x = d(generator);
            double cam_y = d(generator);
            Ray ray = camera->Project(x + cam_x, y + cam_y);
            effect->ModifyRay(ray, generator);
            double frequency = frequencyDistribution(generator);
            Color albedoMultiplier = Color::FromFrequency(frequency);
            albedoMultiplier *= 1 - std::abs(cam_x * cam_y);
            std::stack<Environment> environments;
            environments.push(camera->environment);
            for (int i = 0; i < iteration_limit; i++) {
                Intersection intersection(this, ray);
                Intersect(ray, intersection);
                auto material = intersection.GetMaterial();
                if(intersection) {
                    albedoMultiplier *= (environments.top().attenuation * (-intersection.t)).Exp();
                }
                image->AddPixel(x, y, material.emissive * albedoMultiplier);
                albedoMultiplier *= material.albedo;
                if (albedoMultiplier == Color()) {
                    break;
                }
                ray = intersection.Reflect(ray, albedoMultiplier, frequency, environments, generator);
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

Scene::Scene() : Body(std::make_unique<FlatInterpolator<Material>>(Material())), effect(std::make_unique<Effect>()) {}

Scene::Scene(const std::string & fileName) : Scene() {
    //SetMaterialInterpolator(std::make_unique<FlatInterpolator<Material>>(Material(Color(0,0,0), Color(), false)));
    std::ifstream file(fileName);
    std::string line;
    materialLibrary["whiteDiffuse"].base = Material(Color(0,0,0), Color(0.95,0.95,0.95), false);
    materialLibrary["whiteLight"].base = Material(Color(100,100,100), Color(0,0,0));
    materialLibrary["whiteRough"].base = Material(Color(0,0,0), Color(0.95,0.92,0.90), true, 0.0002);
    TexturedMaterial * current_material = &materialLibrary["whiteDiffuse"];
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
            auto sphere = std::make_unique<Sphere>(transforms.top().Apply(Vector3D(x, y, z)), radius, current_material->base);
            if(current_material->albedoTexture && current_material->albedoTexture->isSpatial()) {
                sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                        std::make_unique<PassThroughInterpolator>(),
                        current_material
                ));
            }
            else {
                sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                        std::make_unique<SpherePolarInterpolator>(),
                        current_material
                ));
            }
            AddBody(std::move(sphere));
        }
        else if(command == "triangle") {
            double ax, ay, az, bx, by, bz, cx, cy, cz;
            lineStream >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz;
            AddBody(std::make_unique<Triangle>(transforms.top().Apply(Vector3D(ax, ay, az)), transforms.top().Apply(Vector3D(bx, by, bz)), transforms.top().Apply(Vector3D(cx, cy, cz)), current_material->base));
        }
        else if(command == "sphere_lens") {
            double x, y, z, radius, rimRadius;
            lineStream >> x >> y >> z >> radius >> rimRadius;
            auto sphere = std::make_unique<SphereEnd>(transforms.top().Apply(Vector3D(x, y, z)), radius, transforms.top().ApplyWithoutTranslation(Vector3D(0,0,1)), rimRadius, current_material->base);
            if(current_material->albedoTexture && current_material->albedoTexture->isSpatial()) {
                sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                        std::make_unique<PassThroughInterpolator>(),
                        current_material
                ));
            }
            else {
                sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                        std::make_unique<SpherePolarInterpolator>(),
                        current_material
                ));
            }
            AddBody(std::move(sphere));
        }
        else if(command == "cylinder") {
            double radius, low, high;
            lineStream >> radius >> low >> high;
            auto sphere = std::make_unique<Cylinder>(transforms.top().Apply(Vector3D(0, 0, 0)),  transforms.top().ApplyWithoutTranslation(Vector3D(0,0,1)), radius, low, high, current_material->base);
            if(current_material->albedoTexture && current_material->albedoTexture->isSpatial()) {
                sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                        std::make_unique<PassThroughInterpolator>(),
                        current_material
                ));
            }
            else {
                sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                        std::make_unique<SpherePolarInterpolator>(),
                        current_material
                ));
            }
            AddBody(std::move(sphere));
        }
        else if(command == "circle") {
            double ox, oy, oz, nx, ny, nz, radius;
            lineStream >> ox >> oy >> oz >> nx >> ny >> nz >> radius;
            AddBody(std::make_unique<PlaneCircle>(transforms.top().Apply(Vector3D(ox, oy, oz)), transforms.top().ApplyWithoutTranslation(Vector3D(nx, ny, nz)), radius, current_material->base));
        }
        else if(command == "lens") {
            double radius, depth;
            lineStream >> radius >> depth;
            {
                auto cylinder = std::make_unique<Cylinder>(transforms.top().Apply(Vector3D(0, 0, 0)), transforms.top().ApplyWithoutTranslation(Vector3D(0, 0, 1)), radius, -depth / 2, depth / 2, current_material->base);
                if(current_material->albedoTexture && current_material->albedoTexture->isSpatial()) {
                    cylinder->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                            std::make_unique<PassThroughInterpolator>(),
                            current_material
                    ));
                }
                else {
                    cylinder->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                            std::make_unique<SpherePolarInterpolator>(),
                            current_material
                    ));
                }
                AddBody(std::move(cylinder));
            }
            // read one end
            for(int i = 0; i < 2; i++) {
                std::string type;
                lineStream >> type;
                if(type == "flat") {
                    AddBody(std::make_unique<PlaneCircle>(transforms.top().Apply(Vector3D(0, 0, i == 0 ? -depth/2 : depth/2)), transforms.top().ApplyWithoutTranslation(Vector3D(0, 0, i == 0 ? -1 : 1)), radius, current_material->base));
                }
                else if(type == "sphere") {
                    double curvatureRadius;
                    lineStream >> curvatureRadius;
                    double a = sqrt(curvatureRadius * curvatureRadius - radius * radius);
                    double q = (std::abs(curvatureRadius) - a) * (std::signbit(curvatureRadius) ? -1 : 1);
                    auto sphere = std::make_unique<SphereEnd>(transforms.top().Apply(Vector3D(0, 0, i == 0 ? -depth/2 - q : depth/2 + q)), curvatureRadius, transforms.top().ApplyWithoutTranslation(Vector3D(0, 0, i == 0 ? -1 : 1)), radius, current_material->base);
                    if(current_material->albedoTexture && current_material->albedoTexture->isSpatial()) {
                        sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                                std::make_unique<PassThroughInterpolator>(),
                                current_material
                        ));
                    }
                    else {
                        sphere->SetMaterialInterpolator(std::make_unique<TextureInterpolator>(
                                std::make_unique<SpherePolarInterpolator>(),
                                current_material
                        ));
                    }
                    AddBody(std::move(sphere));
                }
            }
        }
        else if(command == "current_material") {
            std::string newCurrentMaterialName;
            lineStream >> newCurrentMaterialName;
            auto it = materialLibrary.find(newCurrentMaterialName);
            if(it == materialLibrary.end()) {
                throw std::runtime_error("Material not found, GTFO!");
            }
            current_material = &(it->second);
        }
        else if(command == "load_material_library") {
            std::string mtlFileName;
            lineStream >> mtlFileName;
            LoadMTL(mtlFileName);
        }
        else if(command == "background_texture") {
            std::string texturePath;
            lineStream >> texturePath;
            std::shared_ptr<Image> bg(new ImageJPEG(texturePath, 1, 95));
            bg->SetRepeatY(false);
            SetMaterialInterpolator(
                std::make_unique<LegacyTextureInterpolator>(std::make_unique<SpherePolarInterpolator>(),
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
            camera = std::make_unique<PerspectiveCamera>(transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)),
                                              transforms.top().ApplyWithoutTranslation(Vector3D(dir_x, dir_y, dir_z)),
                                              transforms.top().ApplyWithoutTranslation(Vector3D(up_x, up_y, up_z)), image_width, image_height, fov);
        }
        else if(command == "camera_ortho") {
            double eye_x, eye_y, eye_z, dir_x, dir_y, dir_z, up_x, up_y, up_z, horizontalExtent;
            lineStream >> eye_x >> eye_y >> eye_z >> dir_x >> dir_y >> dir_z >> up_x >> up_y >> up_z >> image_width >> image_height >> horizontalExtent;
            camera = std::make_unique<OrthogonalCamera>(transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)),
                                                         transforms.top().ApplyWithoutTranslation(Vector3D(dir_x, dir_y, dir_z)),
                                                         transforms.top().ApplyWithoutTranslation(Vector3D(up_x, up_y, up_z)), image_width, image_height, horizontalExtent);
        }
        else if(command == "camera_sphere") {
            double eye_x, eye_y, eye_z;
            lineStream >> eye_x >> eye_y >> eye_z >> image_width >> image_height;
            camera = std::make_unique<SphericalCamera>(transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)), image_width, image_height);
        }
        else if(command == "camera_cubemap") {
            double eye_x, eye_y, eye_z;
            lineStream >> eye_x >> eye_y >> eye_z >> image_width >> image_height;
            camera = std::make_unique<CubeMapCamera>(transforms.top().Apply(Vector3D(eye_x, eye_y, eye_z)), image_width, image_height);
        }
        else if(command == "camera_env") {
            std::string newCurrentMaterialName;
            lineStream >> newCurrentMaterialName;
            auto it = materialLibrary.find(newCurrentMaterialName);
            if(it == materialLibrary.end()) {
                throw std::runtime_error("Material not found, GTFO!");
            }
            camera->environment.attenuation = it->second.base.attenuation;
            camera->environment.refractiveIndex = it->second.base.refractiveIndex;
        }
        else if(command == "dof") {
            double focus, spread;
            lineStream >> focus >> spread;
            if(focus == 0 || spread == 0) {
                effect = std::make_unique<Effect>();
            }
            else {
                effect = std::make_unique<DepthOfFieldEffect>(focus, spread);
            }
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

void Scene::LoadMTL(const std::string & fileName) {
    TexturedMaterial * current_material = &materialLibrary["whiteDiffuse"];
    std::ifstream file(fileName);
    std::string line;
    while(std::getline(file, line)) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(command == "newmtl") {
            std::string newCurrentMaterialName;
            lineStream >> newCurrentMaterialName;
            current_material = &materialLibrary[newCurrentMaterialName];
        }
        else if(command == "Kd") { // albedo
            lineStream >> current_material->base.albedo.r >> current_material->base.albedo.g >> current_material->base.albedo.b;
        }
        else if(command == "Ka") { // emission
            lineStream >> current_material->base.emissive.r >> current_material->base.emissive.g >> current_material->base.emissive.b;
        }
        else if(command == "Km") { // metalness
            double m;
            lineStream >> m;
            current_material->base.metalness = (m > 0.5);
        }
        else if(command == "Kr") { // roughness
            lineStream >> current_material->base.roughness;
        }
        else if(command == "Kf") { // refractive index
            lineStream >> current_material->base.refractiveIndex;
            double dispersionFactor;
            lineStream >> dispersionFactor;
            if(lineStream) {
                current_material->base.dispersionFactor = dispersionFactor;
            }
        }
        else if(command == "Ko") { // opacity
            lineStream >> current_material->base.opacity;
        }
        else if(command == "Kb") {
            lineStream >> current_material->base.attenuation.r >> current_material->base.attenuation.g >> current_material->base.attenuation.b;
        }
        else if(command == "map_Kd") { // albedo texture
            std::string texturePath;
            lineStream >> texturePath;
            current_material->albedoTexture = std::make_shared<ImageJPEG>(texturePath, 1, 95);
        }
        else if(command == "map_Ka") { // emission texture
            std::string texturePath;
            lineStream >> texturePath;
            current_material->emissiveTexture = std::make_shared<ImageJPEG>(texturePath, 1, 95);
        }
        else if(command == "map_Km") { // metalness texture
            std::string texturePath;
            lineStream >> texturePath;
            current_material->metalnessTexture = std::make_shared<ImageJPEG>(texturePath, 0, 95);
        }
        else if(command == "map_Kr") { // roughness texture
            std::string texturePath;
            lineStream >> texturePath;
            current_material->roughnessTexture = std::make_shared<ImageJPEG>(texturePath, 0, 95);
        }
        else if(command == "map_Kn") { // roughness texture
            std::string texturePath;
            lineStream >> texturePath;
            current_material->normalTexture = std::make_shared<ImageJPEG>(texturePath, 0, 95);
        }
        else if(command == "shader_Kd") { // albedo texture
            std::string texturePath;
            lineStream >> texturePath;
            if(texturePath == "wood") {
                current_material->albedoTexture = std::make_shared<Wood>();
            }
            else if(texturePath == "marble") {
                current_material->albedoTexture = std::make_shared<Marble>();
            }
            else if(texturePath == "voronoi") {
                current_material->albedoTexture = std::make_shared<Voronoi>();
            }
        }
    }
}

const TexturedMaterial * Scene::GetMaterial(const std::string &materialName) const {
    return &materialLibrary.at(materialName);
}

Scene::~Scene() = default;
