#pragma once
#include "../texture/texture.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>

struct Material {
    std::string m_name;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_shininess;
    Texture m_diffuseMap;
    Texture m_specularMap;

    void Bind();
};
