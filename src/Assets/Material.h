#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    Texture diffuseMap;
    Texture specularMap;
    Texture normalMap;
};
