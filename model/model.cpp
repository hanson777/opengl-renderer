#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include <stb_image.h>
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
    std::cout << "directory: " << m_directory << std::endl;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), m_directory.c_str());

    if (!success || !err.empty()) {
        std::cout << "[ERROR::MODEL] " << err << std::endl;
        return;
    }

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
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(attrib.vertices.size() / 3);
	indices.reserve(shapes[0].mesh.indices.size());
	uniqueVertices.reserve(attrib.vertices.size() / 3);

    std::cout << "Total face vertices: " << attrib.vertices.size() / 3 << std::endl;
	std::cout << "Total face indices: " << shapes[0].mesh.indices.size() / 3 << std::endl;
 
	for (auto& shape : shapes) {
		for (auto& index : shape.mesh.indices) {
			IndexKey key{ index.vertex_index, index.normal_index, index.texcoord_index };

            auto it = uniqueVertices.find(key);
            if (it != uniqueVertices.end()) {
                indices.push_back(it->second);
            } else {
                Vertex v{};
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

                uint32_t newIndex = static_cast<uint32_t>(vertices.size());
				uniqueVertices[key] = newIndex;
				vertices.push_back(v);
				indices.push_back(newIndex);
            }
		}
	}

    std::vector<Texture> textures = loadMaterialTextures(materials);
    std::cout << "material size: " << materials.size() << std::endl;
    std::cout << "texture size: " << textures.size() << std::endl;

    Mesh mesh(vertices, indices, textures);
    m_meshes.push_back(mesh);
}

uint32_t createWhiteTexture() {
	uint32_t id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	unsigned char white[] = { 255, 255, 255, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return id;
}

std::vector<Texture> Model::loadMaterialTextures(std::vector<tinyobj::material_t> materials) {
    std::vector<Texture> textures;
	for (auto& mat : materials) {
		if (!mat.diffuse_texname.empty())
			textures.push_back(loadMaterialTexture(mat, TextureType::Diffuse));
		else
			textures.push_back({ createWhiteTexture(), TextureType::Diffuse });

		if (!mat.specular_texname.empty())
			textures.push_back(loadMaterialTexture(mat, TextureType::Specular));
		else
			textures.push_back({ createWhiteTexture(), TextureType::Specular });
	}
    return textures;
}

Texture Model::loadMaterialTexture(tinyobj::material_t mat, TextureType type) {
    std::string path;
    if (type == TextureType::Diffuse) path = mat.diffuse_texname;
    else if (type == TextureType::Specular) path = mat.specular_texname;

    Texture texture;
    texture.id = loadTextureFile(m_directory + path);
    texture.type = type;
    std::cout << "Loaded texture id: " << texture.id << " path: " << path << std::endl;
    return texture;
}

uint32_t Model::loadTextureFile(std::string path) {
    uint32_t textureId;
    glGenTextures(1, &textureId);

    int width, height, numChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
    if (!data) { 
        std::cout << "[ERROR::MODEL] failed to load texture at path: " << path << std::endl; 
        stbi_image_free(data);
        return textureId;
    }

    GLint internalFormat = (numChannels == 4) ? GL_RGBA8 : GL_RGB8; // How the GPU stores texture in VRAM
    GLenum pixelFormat   = (numChannels == 4) ? GL_RGBA  : GL_RGB;  // describes CPU-side data

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

    std::cout << "textureId: " << textureId << std::endl;
    return textureId;
}

void Model::draw(Shader& shader, bool loadMats) {
    for (Mesh& mesh : m_meshes) {
        mesh.draw(shader, loadMats);
    }
}
