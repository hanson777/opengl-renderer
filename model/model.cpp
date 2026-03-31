#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "model.h"
#include <iostream>
#include <unordered_map>
#include <glad/glad.h>
#include <chrono>

Model::Model(const std::string& path) {
	auto start = std::chrono::high_resolution_clock::now();

    loadModel(path);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	std::cout << "Load time: " << elapsed.count() << "ms\n";
}

void Model::loadModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    m_directory = path.substr(0, path.find_last_of("/\\")) + "/";
    std::cout << "Directory: " << m_directory << std::endl;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), m_directory.c_str());

    if (!success || !err.empty()) {
        std::cout << "[ERROR::MODEL] " << err << std::endl;
        return;
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(attrib.vertices.size() / 3);
	indices.reserve(shapes[0].mesh.indices.size());
	uniqueVertices.reserve(attrib.vertices.size() / 3);

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
	    }

    std::cout << "Material size: " << materials.size() << std::endl;

    Mesh mesh(vertices, indices);
    m_meshes.push_back(mesh);
}

void Model::draw(Shader& shader) {
    for (Mesh& mesh : m_meshes) {
        mesh.draw(shader);
    }
}
