#include "Scene.h"
#include "../Renderer/Renderer.h"
#include "../Assets/AssetManager.h"
#include "../Input/Input.h"
#include <iostream>

#define W_KEY 87
#define S_KEY 83
#define A_KEY 65
#define D_KEY 68
#define SPACE_KEY 32
#define LEFT_SHIFT_KEY 340

namespace Scene {
    Camera g_camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    std::vector<SceneObject> g_sceneObjects;

    void Init() {

        int blankIdx = Renderer::LoadShader("shaders/blankShader.vert", "shaders/blankShader.frag");
        int phongIdx = Renderer::LoadShader("shaders/phongShader.vert", "shaders/phongShader.frag");

        int sponzaIdx = AssetManager::LoadModel("obj/sponza/sponza.obj");
        int cubeIdx = AssetManager::LoadModel("obj/cube/cube.obj");

        SceneObject sponza;
        sponza.m_modelIndex = sponzaIdx;
        sponza.m_shaderIndex = phongIdx;
        sponza.m_scale = glm::vec3(0.1f);
        g_sceneObjects.push_back(sponza);

        SceneObject lightCube;
        lightCube.m_modelIndex = cubeIdx;
        lightCube.m_shaderIndex = blankIdx;
        lightCube.m_scale = glm::vec3(0.25f);
        lightCube.m_position = glm::vec3(0.0f, 2.0f, 0.0f);
        g_sceneObjects.push_back(lightCube);

        g_camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void Update(float deltaTime) {
        if (Input::KeyPressed(W_KEY))
            g_camera.ProcessKeyboard(Camera::Forward, deltaTime);
        if (Input::KeyPressed(S_KEY))
            g_camera.ProcessKeyboard(Camera::Backward, deltaTime);
        if (Input::KeyPressed(A_KEY))
            g_camera.ProcessKeyboard(Camera::Left, deltaTime);
        if (Input::KeyPressed(D_KEY))
            g_camera.ProcessKeyboard(Camera::Right, deltaTime);
        if (Input::KeyPressed(SPACE_KEY))
            Scene::g_camera.ProcessKeyboard(Camera::Up, deltaTime);
        if (Input::KeyPressed(LEFT_SHIFT_KEY))
            Scene::g_camera.ProcessKeyboard(Camera::Down, deltaTime);

        g_camera.ProcessMouseMovement(Input::GetMouseDelta().x, Input::GetMouseDelta().y);
        g_camera.ProcessMouseScroll(Input::GetScrollDelta());

        for (SceneObject& sceneObject : g_sceneObjects) {
            sceneObject.Update(deltaTime);
        }
    }
}


