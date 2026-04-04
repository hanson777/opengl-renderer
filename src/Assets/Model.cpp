#define TINYOBJLOADER_IMPLEMENTATION
#include "Model.h"
#include "AssetManager.h"
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <algorithm>

Model::Model(const std::string& path) {
    auto start = std::chrono::high_resolution_clock::now();

    Load(path);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Load time: " << elapsed.count() << "ms\n";
}

void Model::Load(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    m_directory = path.substr(0, path.find_last_of("/\\")) + "/";
    std::cout << "Directory: " << m_directory << std::endl;
    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), m_directory.c_str());
    if (!success) {
        std::cout << "[ERROR::MODEL] " << err << std::endl;
        return;
    }

    if (!err.empty()) {
        std::cout << "[ERROR::MODEL::NON_FATAL] " << err << std::endl;
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    vertices.reserve(attrib.vertices.size() / 3);
    indices.reserve(shapes[0].mesh.indices.size());
    uniqueVertices.reserve(attrib.vertices.size() / 3);
    m_meshes.reserve(shapes.size());
    m_materials.reserve(materials.size());

    std::cout << "Total face vertices: " << attrib.vertices.size() / 3 << std::endl;
    std::cout << "Total face indices: " << shapes[0].mesh.indices.size() / 3 << std::endl;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex v;
            v.position = { attrib.vertices[3 * index.vertex_index],
                           attrib.vertices[3 * index.vertex_index + 1],
                           attrib.vertices[3 * index.vertex_index + 2] };
            if (index.normal_index >= 0) {
                v.normal = { attrib.normals[3 * index.normal_index],
                             attrib.normals[3 * index.normal_index + 1],
                             attrib.normals[3 * index.normal_index + 2] };
            }
            if (index.texcoord_index >= 0) {
                v.texCoords = { attrib.texcoords[2 * index.texcoord_index],
                                1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };
            }
            if (!uniqueVertices.contains(v)) {
                uniqueVertices[v] = vertices.size();
                vertices.push_back(v);
            }
            indices.push_back(uniqueVertices[v]);
        }

        Mesh mesh(vertices, indices);
        mesh.m_materialId = shape.mesh.material_ids[0];

        m_meshIndices.push_back(AssetManager::g_meshes.size());
        AssetManager::g_meshes.push_back(mesh);

        vertices.clear();
        indices.clear();
        uniqueVertices.clear();
    }
    LoadMaterials(materials);
    InitDefaultMaterial();
}

void Model::LoadMaterials(const std::vector<tinyobj::material_t>& materials) {
    for (const auto& material : materials) {
        Material mat;
        mat.m_name = material.name;
        mat.m_ambient = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
        mat.m_diffuse = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        mat.m_specular = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
        mat.m_shininess = material.shininess;

        Texture tex;
        if (!material.diffuse_texname.empty()) {
            std::string diffusePath = material.diffuse_texname;
            std::replace(diffusePath.begin(), diffusePath.end(), '\\', '/');
            tex.Load(m_directory + diffusePath);
            mat.m_diffuseMap = tex;
        }
        else {
            tex.Load("obj/fallbacks/missing_texture.png");
            mat.m_diffuseMap = tex;
        }

        if (!material.specular_texname.empty()) {
            std::string specularPath = material.specular_texname;
            std::replace(specularPath.begin(), specularPath.end(), '\\', '/');
            tex.Load(m_directory + specularPath);
            mat.m_specularMap = tex;
        }

        m_materials.push_back(mat);
    }
}

void Model::InitDefaultMaterial() {
    m_defaultMaterial.m_name = "Default";
    m_defaultMaterial.m_ambient = glm::vec3(0.1f);
    m_defaultMaterial.m_diffuse = glm::vec3(1.0f);
    m_defaultMaterial.m_specular = glm::vec3(0.0f);
    m_defaultMaterial.m_shininess = 1.0f;

    Texture tex;
    tex.Load("obj/fallbacks/missing_texture.png");
    m_defaultMaterial.m_diffuseMap = tex;
    tex.GenerateWhiteTexture();
    m_defaultMaterial.m_specularMap = tex;
}
