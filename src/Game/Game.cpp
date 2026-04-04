#include "Game.h"
#include "../Renderer/Renderer.h"
#include "../Assets/AssetManager.h"
#include "../Scene/Scene.h"

namespace Game {
    void Init() {
        int blankIdx = Renderer::LoadShader("shaders/blankShader.vert", "shaders/blankShader.frag");
        int phongIdx = Renderer::LoadShader("shaders/phongShader.vert", "shaders/phongShader.frag");

        int sponzaIdx = AssetManager::LoadModel("res/sponza/sponza.obj");
        int cubeIdx = AssetManager::LoadModel("res/cube/cube.obj");

        SceneObject sponza;
        sponza.m_modelIndex = sponzaIdx;
        sponza.m_shaderIndex = phongIdx;
        sponza.m_scale = glm::vec3(0.1f);
        Scene::g_sceneObjects.push_back(sponza);

        SceneObject lightCube;
        lightCube.m_modelIndex = cubeIdx;
        lightCube.m_shaderIndex = blankIdx;
        lightCube.m_scale = glm::vec3(0.25f);
        lightCube.m_position = glm::vec3(0.0f, 2.0f, 0.0f);
        Scene::g_sceneObjects.push_back(lightCube);

        Scene::g_camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}
