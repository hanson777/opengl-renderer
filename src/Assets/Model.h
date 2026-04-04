#pragma once
#include "Mesh.h"
#include "Material.h"
#include <tiny_obj_loader.h>

class Model {
public:
    Model(const std::string& filepath);

    std::vector<int> m_meshIndices;

    const std::vector<Material>& GetMaterials() const { return m_materials; }
    std::vector<Material>& GetMaterials() { return m_materials; }

    const Material& GetDefaultMaterial() const { return m_defaultMaterial; }
    Material& GetDefaultMaterial() { return m_defaultMaterial; }

private:
    std::vector<Material> m_materials;
    Material m_defaultMaterial;
    std::string m_directory;

    void Load(const std::string& path);
    void LoadMaterials(const std::vector<tinyobj::material_t>& materials);
    void InitDefaultMaterial();
};
