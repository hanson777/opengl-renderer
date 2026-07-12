#pragma once
#include "Material.h"
#include <tiny_obj_loader.h>
#include <vector>

struct Model {
    Model(const std::string& filepath);

    std::vector<int> m_meshIndices;
    std::vector<Material> m_materials;

    Material m_defaultMaterial;

    std::string m_directory;

    const std::vector<Material>& GetMaterials()       const       { return m_materials; }
    const Material&              GetDefaultMaterial() const       { return m_defaultMaterial; }

    void Load(const std::string& path);
    void LoadMaterials(const std::vector<tinyobj::material_t>& materials);
    void InitDefaultMaterial();
};
