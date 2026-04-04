#include "Scene.h"

std::vector<SceneObject> Scene::g_sceneObjects;
Camera Scene::g_camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

void Scene::Update(float deltaTime) {
	for (SceneObject& sceneObject : g_sceneObjects) {
		sceneObject.Update(deltaTime);
	}
}
