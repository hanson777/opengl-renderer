#pragma once
#include <tiny_obj_loader.h>
#include "mesh/mesh.h"

class Model {
public:
    Model(const std::string& path);
    void draw(Shader& shader, bool loadMats);

private:
    std::vector<Mesh> m_meshes;
    void loadModel(const std::string& path);
    std::vector<Texture> loadMaterialTextures(std::vector<tinyobj::material_t> materials);
    Texture loadMaterialTexture(tinyobj::material_t mat, TextureType type);
    uint32_t loadTextureFile(std::string path);
    std::string m_directory;
};
