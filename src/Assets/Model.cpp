#define TINYOBJLOADER_IMPLEMENTATION
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "AssetManager.h"
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <string>

Model::Model(const std::string& filepath) {
    auto start = std::chrono::high_resolution_clock::now();

    Load(filepath);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Load time: " << elapsed.count() << "ms\n";
}

void Model::Load(const std::string& path) {
    std::cout << "Loading model " << path << '\n';

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    m_directory = path.substr(0, path.find_last_of("/\\")) + "/";

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), m_directory.c_str())) {
        std::cout << "[ERROR::MODEL] " << err << std::endl;
        return;
    }

    if (!err.empty()) {
        std::cout << "[ERROR::MODEL::NON_FATAL] " << err << std::endl;
    }

    std::cout << "Total face vertices: " << attrib.vertices.size() / 3 << '\n';
    std::cout << "Total face indices: " << shapes[0].mesh.indices.size() / 3 << '\n';

    for (const auto& shape : shapes) {

        std::unordered_map<int, std::vector<tinyobj::index_t>> materialGroups;
        for (int i = 0; i < shape.mesh.material_ids.size(); i++) {
            int matId = shape.mesh.material_ids[i];
            for (int j = 0; j < 3; j++) {
                materialGroups[matId].push_back(shape.mesh.indices[3 * i + j]);
            }
        }

        for (const auto& [matId, vertexIndices] : materialGroups) {

            std::unordered_map<Vertex, uint32_t> uniqueVertices;
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            vertices.reserve(vertexIndices.size());
            indices.reserve(vertexIndices.size());
            uniqueVertices.reserve(vertexIndices.size());

            for (const auto& index : vertexIndices) {
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
                    v.uv = { attrib.texcoords[2 * index.texcoord_index],
                             1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };
                }
                if (!uniqueVertices.contains(v)) {
                    uniqueVertices[v] = vertices.size();
                    vertices.push_back(v);
                }
                indices.push_back(uniqueVertices[v]);
            }

            for (int i = 0; i < indices.size(); i += 3) {
                Vertex& v0 = vertices[indices[i]];
                Vertex& v1 = vertices[indices[i + 1]];
                Vertex& v2 = vertices[indices[i + 2]];

                glm::vec3 e0 = v1.position - v0.position;
                glm::vec3 e1 = v2.position - v0.position;

                glm::vec2 u0 = v1.uv - v0.uv;
                glm::vec2 u1 = v2.uv - v0.uv;

                float f = (u0.x * u1.y) - (u1.x * u0.y);
                if (fabs(f) < 1e-6f) continue;
                f = 1.0 / f;
                glm::vec3 tangent = f * (u1.y * e0 - u0.y * e1);

                v0.tangent += tangent;
                v1.tangent += tangent;
                v2.tangent += tangent;
            }

            MeshData data;
            data.vertices = std::move(vertices);
            data.indices = std::move(indices);
            data.materialId = matId;

            m_meshIndices.push_back(AssetManager::g_meshData.size());
            AssetManager::g_meshData.push_back(std::move(data));
        }
    }
    LoadMaterials(materials);
    InitDefaultMaterial();
}

void Model::LoadMaterials(const std::vector<tinyobj::material_t>& materials) {
    m_materials.reserve(materials.size());
    for (const auto& material : materials) {
        Material mat;
        mat.name = material.name;
        mat.ambient = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
        mat.diffuse = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        mat.specular = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
        mat.shininess = material.shininess;

        if (!material.diffuse_texname.empty()) {
            std::string path = material.diffuse_texname;
            std::replace(path.begin(), path.end(), '\\', '/');
            mat.diffuseMap.Load(m_directory + path);
        } else {
            mat.diffuseMap.Load("res/textures/fallbacks/missing_texture.png");
        }

        if (!material.specular_texname.empty()) {
            std::string path = material.specular_texname;
            std::replace(path.begin(), path.end(), '\\', '/');
            mat.specularMap.Load(m_directory + path);
        }

        if (!material.normal_texname.empty()) {
            std::string path = material.normal_texname;
            std::replace(path.begin(), path.end(), '\\', '/');
            mat.normalMap.Load(m_directory + path);
        }

        m_materials.push_back(mat);
    }
}

void Model::InitDefaultMaterial() {
    m_defaultMaterial.name = "Default";
    m_defaultMaterial.ambient = glm::vec3(0.1f);
    m_defaultMaterial.diffuse = glm::vec3(1.0f);
    m_defaultMaterial.specular = glm::vec3(0.0f);
    m_defaultMaterial.shininess = 1.0f;

    m_defaultMaterial.diffuseMap.Load("res/textures/fallbacks/missing_texture.png");
    m_defaultMaterial.specularMap.GenerateWhiteTexture();
}
