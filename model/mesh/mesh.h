#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../../shader/shader.h"

enum class TextureType { Diffuse, Specular, Normal, Ambient };

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
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
