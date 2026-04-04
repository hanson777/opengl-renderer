#pragma once
#include <glm/glm.hpp>

class Model;

struct SceneObject {
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    Model* m_model = nullptr;

    glm::mat4 GetModelMatrix();
    void Update(float deltaTime);
};
