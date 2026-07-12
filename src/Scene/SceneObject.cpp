#include "SceneObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 SceneObject::GetModelMatrix() const {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, scale);
    return model;
}

void SceneObject::Update(float deltaTime) {
}
