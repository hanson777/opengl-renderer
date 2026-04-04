#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

struct Material {
    std::string m_name;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_shininess;
    Texture m_diffuseMap;
    Texture m_specularMap;
};
