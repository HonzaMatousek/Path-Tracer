#include "CommandLoadMaterialLibrary.h"

#include <istream>
#include <fstream>
#include <sstream>

#include "../material/TexturedMaterial.h"
#include "../image/ImageJPEG.h"
#include "../shader/Wood.h"
#include "../shader/Marble.h"
#include "../shader/Voronoi.h"

void CommandLoadMaterialLibrary::Execute(SceneBuilder &sceneBuilder, std::istream &lineStream) const {
    std::string mtlFileName;
    lineStream >> mtlFileName;
    TexturedMaterial * current_material = sceneBuilder.GetMaterial("whiteDiffuse");
    std::ifstream file(mtlFileName);
    std::string line;
    while(std::getline(file, line)) {
        std::string command;
        std::istringstream mtlLineStream(line);
        mtlLineStream >> command;
        if(command == "newmtl") {
            std::string newCurrentMaterialName;
            mtlLineStream >> newCurrentMaterialName;
            current_material = sceneBuilder.NewMaterial(newCurrentMaterialName);
        }
        else if(command == "Kd") { // albedo
            mtlLineStream >> current_material->base.albedo.r >> current_material->base.albedo.g >> current_material->base.albedo.b;
        }
        else if(command == "Ka") { // emission
            mtlLineStream >> current_material->base.emissive.r >> current_material->base.emissive.g >> current_material->base.emissive.b;
        }
        else if(command == "Km") { // metalness
            double m;
            mtlLineStream >> m;
            current_material->base.metalness = (m > 0.5);
        }
        else if(command == "Kr") { // roughness
            mtlLineStream >> current_material->base.roughness;
        }
        else if(command == "Kf") { // refractive index
            mtlLineStream >> current_material->base.refractiveIndex;
            double dispersionFactor;
            mtlLineStream >> dispersionFactor;
            if(mtlLineStream) {
                current_material->base.dispersionFactor = dispersionFactor;
            }
        }
        else if(command == "Ko") { // opacity
            mtlLineStream >> current_material->base.opacity;
        }
        else if(command == "Kb") {
            mtlLineStream >> current_material->base.attenuation.r >> current_material->base.attenuation.g >> current_material->base.attenuation.b;
        }
        else if(command == "map_Kd") { // albedo texture
            std::string texturePath;
            mtlLineStream >> texturePath;
            current_material->albedoTexture = std::make_shared<ImageJPEG>(texturePath, 1, 95);
        }
        else if(command == "map_Ka") { // emission texture
            std::string texturePath;
            mtlLineStream >> texturePath;
            current_material->emissiveTexture = std::make_shared<ImageJPEG>(texturePath, 1, 95);
        }
        else if(command == "map_Km") { // metalness texture
            std::string texturePath;
            mtlLineStream >> texturePath;
            current_material->metalnessTexture = std::make_shared<ImageJPEG>(texturePath, 0, 95);
        }
        else if(command == "map_Kr") { // roughness texture
            std::string texturePath;
            mtlLineStream >> texturePath;
            current_material->roughnessTexture = std::make_shared<ImageJPEG>(texturePath, 0, 95);
        }
        else if(command == "map_Kn") { // roughness texture
            std::string texturePath;
            mtlLineStream >> texturePath;
            current_material->normalTexture = std::make_shared<ImageJPEG>(texturePath, 0, 95);
        }
        else if(command == "shader_Kd") { // albedo texture
            std::string texturePath;
            mtlLineStream >> texturePath;
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