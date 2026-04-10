#pragma once
#include <glm/glm.hpp>

enum class LightType { Directional = 0, Point = 1, Spot = 2 };

struct Light {
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float radius;
};
