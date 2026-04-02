#pragma once
#include "../mesh/mesh.h"
#include "../material/material.h"
#include <tiny_obj_loader.h>

class Model {
public:
    Model(const std::string& path);

    void Draw(Shader& shader);

private:
    std::vector<Mesh> m_meshes;
    std::vector<Material> m_materials;
    std::string m_directory;

    void LoadModel(const std::string& path);
    void LoadMaterials(const std::vector<tinyobj::material_t>& materials);
};