#include "Scene.h"

namespace Scene {
    Camera g_camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    std::vector<SceneObject> g_sceneObjects;

    void Update(float deltaTime) {
        g_camera.Update(deltaTime);

        for (SceneObject& sceneObject : g_sceneObjects) {
            sceneObject.Update(deltaTime);
        }
    }
}
