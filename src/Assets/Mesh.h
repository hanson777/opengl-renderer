#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
    glm::vec3 tangent = glm::vec3(0.0f);

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && uv == other.uv;
    };
};

template<>
struct std::hash<Vertex> {
    size_t operator()(const Vertex& v) const noexcept {
        size_t h1 = hash<glm::vec3>()(v.position);
        size_t h2 = hash<glm::vec3>()(v.normal);
        size_t h3 = hash<glm::vec2>()(v.uv);
        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct MeshData {
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    int m_materialId = -1;
};

struct Mesh {
      uint32_t m_vao = 0;
      uint32_t m_vbo = 0;
      uint32_t m_ebo = 0;
      uint32_t m_indexCount = 0;
      int m_materialId = -1;
};
