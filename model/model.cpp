#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "model.h"
#include <iostream>
#include <unordered_map>

Model::Model(const std::string& path) {
    loadModel(path);
}

void Model::loadModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());
    if (!success) {
        std::cout << "[ERROR::MODEL] " << err << std::endl;
        return;
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // Use a packed struct as key instead of slow string concatenation
    struct IndexKey {
        int v, n, t;
        bool operator==(const IndexKey& o) const {
            return v == o.v && n == o.n && t == o.t;
        }
    };
    struct IndexKeyHash {
        size_t operator()(const IndexKey& k) const {
            return ((size_t)k.v * 73856093) ^ ((size_t)k.n * 19349663) ^ ((size_t)k.t * 83492791);
        }
    };
    std::unordered_map<IndexKey, uint32_t, IndexKeyHash> uniqueVertices;

    for (auto& shape : shapes) {
        for (auto& index : shape.mesh.indices) {
            IndexKey key{ index.vertex_index, index.normal_index, index.texcoord_index };

            auto it = uniqueVertices.find(key);
            if (it != uniqueVertices.end()) {
                indices.push_back(it->second);
            } else {
                Vertex v{};
                v.position = { attrib.vertices[3 * index.vertex_index + 0],
                               attrib.vertices[3 * index.vertex_index + 1],
                               attrib.vertices[3 * index.vertex_index + 2] };
                if (index.normal_index >= 0) {
                    v.normal = { attrib.normals[3 * index.normal_index + 0],
                                 attrib.normals[3 * index.normal_index + 1],
                                 attrib.normals[3 * index.normal_index + 2] };
                }
                if (index.texcoord_index >= 0) {
                    v.texCoords = { attrib.texcoords[2 * index.texcoord_index + 0],
                                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };
                }

                uint32_t newIndex = static_cast<uint32_t>(vertices.size());
                uniqueVertices[key] = newIndex;
                vertices.push_back(v);
                indices.push_back(newIndex);
            }
        }
    }

    std::vector<Texture> textures;
    Mesh mesh(vertices, indices, textures);
    m_meshes.push_back(mesh);
}

void Model::draw(Shader& shader) {
    for (Mesh& mesh : m_meshes) {
        mesh.draw(shader);
    }
}
