#pragma once
#include <glm/glm.hpp>

struct SceneObject {
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    int m_modelIndex = -1;
    int m_shaderIndex = -1;

    glm::mat4 GetModelMatrix();
    void Update(float deltaTime);
};
