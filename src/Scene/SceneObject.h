#pragma once
#include <glm/glm.hpp>

struct SceneObject {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    int modelIndex = -1;
    int shaderIndex = -1;

    glm::mat4 GetModelMatrix() const;
    void Update(float deltaTime);
};

struct ScreenSpaceObject {
    int primitiveIndex = -1;
    int shaderIndex = -1;
};