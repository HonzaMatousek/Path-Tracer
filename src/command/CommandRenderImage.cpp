#include "CommandRenderImage.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>

#include "../camera/Camera.h"
#include "../effect/Effect.h"
#include "../math/Intersection.h"
#include "../body/Scene.h"
#include "../image/ImageJPEG.h"

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

inline void RenderCore(std::mt19937& generator, LineEmployer & lineEmployer, SceneBuilder &sceneBuilder) {
    std::uniform_real_distribution<double> d(-0.5,0.5);
    std::uniform_real_distribution<double> frequencyDistribution(frequencyR / 1.5, frequencyB * 1.5);
    for (int y = lineEmployer.getLine(); y < sceneBuilder.image_height; y = lineEmployer.getLine()) {
        for (int x = 0; x < sceneBuilder.image_width; x++) {
            double cam_x = d(generator);
            double cam_y = d(generator);
            Ray ray = sceneBuilder.camera->Project(x + cam_x, y + cam_y);
            sceneBuilder.effect->ModifyRay(ray, generator);
            double frequency = frequencyDistribution(generator);
            Color albedoMultiplier = Color::FromFrequency(frequency);
            albedoMultiplier *= 1 - std::abs(cam_x * cam_y);
            std::stack<Environment> environments;
            environments.push(sceneBuilder.camera->environment);
            for (int i = 0; i < iteration_limit; i++) {
                Intersection intersection(&sceneBuilder.scene, ray);
                sceneBuilder.scene.Intersect(ray, intersection);
                auto material = intersection.GetMaterial();
                if(intersection) {
                    albedoMultiplier *= (environments.top().attenuation * (-intersection.t)).Exp();
                }
                sceneBuilder.image->AddPixel(x, y, material.emissive * albedoMultiplier);
                albedoMultiplier *= material.albedo;
                if (albedoMultiplier == Color()) {
                    break;
                }
                ray = intersection.Reflect(ray, albedoMultiplier, frequency, environments, generator);
            }
        }
    }
}

void CommandRenderImage::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    std::string outputFileName;
    double exposition;
    int samples;
    lineStream >> outputFileName >> samples >> exposition;
    sceneBuilder.image = std::make_unique<ImageJPEG>(sceneBuilder.image_width, sceneBuilder.image_height, 95);
    auto start = std::chrono::steady_clock::now();
    std::cout << "Compiling KD tree..." << std::endl;
    sceneBuilder.CompileScene();
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
            threads.emplace_back(std::thread(RenderCore, std::ref(generators[i]), std::ref(lineEmployer), std::ref(sceneBuilder)));
        }
        for(auto & thread : threads) {
            thread.join();
        }
        std::cout << (sample + 1) << "/" << samples << std::endl;
    }
    auto stop = std::chrono::steady_clock::now();
    std::cout << "Finished, took " << ((stop - start).count() / 1e9) <<  " s." << std::endl;
    sceneBuilder.image->Save(outputFileName, exposition / samples);
}
