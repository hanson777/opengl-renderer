#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "../../shader/shader.h"

enum class TextureType { Diffuse, Specular, Normal, Ambient };

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && texCoords == other.texCoords;
    };
};

template<>
struct std::hash<Vertex> {
    size_t operator()(const Vertex& v) const noexcept {
        size_t h1 = hash<glm::vec3>()(v.position);
        size_t h2 = hash<glm::vec3>()(v.normal);
        size_t h3 = hash<glm::vec2>()(v.texCoords);
        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct Texture {
	uint32_t id;
	TextureType type;
};

class Mesh {
  public:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture> m_textures;

    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
    void draw(Shader& shader, bool loadMats);
  private:
    uint32_t m_vao;
    uint32_t m_vbo;
    uint32_t m_ebo;
    void setupMesh();
};
