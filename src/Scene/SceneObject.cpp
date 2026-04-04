#include "SceneObject.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 SceneObject::GetModelMatrix() {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 r = glm::mat4(1.0f);
    r = glm::rotate(r, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
    r = glm::rotate(r, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
    r = glm::rotate(r, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
    glm::mat4 s = glm::scale(glm::mat4(1.0f), m_scale);
    return t * r * s;
}

void SceneObject::Update(float deltaTime) {
}
