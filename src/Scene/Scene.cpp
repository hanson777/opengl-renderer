#include "Scene.h"
#include "../Input/Input.h"

std::vector<SceneObject> Scene::g_sceneObjects;
Camera Scene::g_camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

void Scene::Update(float deltaTime) {
    if (Input::KeyPressed(GLFW_KEY_W))
        g_camera.ProcessKeyboard(Camera::Forward, deltaTime);
    if (Input::KeyPressed(GLFW_KEY_S))
        g_camera.ProcessKeyboard(Camera::Backward, deltaTime);
    if (Input::KeyPressed(GLFW_KEY_A))
        g_camera.ProcessKeyboard(Camera::Left, deltaTime);
    if (Input::KeyPressed(GLFW_KEY_D))
        g_camera.ProcessKeyboard(Camera::Right, deltaTime);
    if (Input::KeyPressed(GLFW_KEY_SPACE))
        Scene::g_camera.ProcessKeyboard(Camera::Up, deltaTime);
    if (Input::KeyPressed(GLFW_KEY_LEFT_SHIFT))
        Scene::g_camera.ProcessKeyboard(Camera::Down, deltaTime);

    g_camera.ProcessMouseMovement(Input::GetMouseDelta().x, Input::GetMouseDelta().y);
    g_camera.ProcessMouseScroll(Input::GetScrollDelta());

	for (SceneObject& sceneObject : g_sceneObjects) {
		sceneObject.Update(deltaTime);
	}
}
