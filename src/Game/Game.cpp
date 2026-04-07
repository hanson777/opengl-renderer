#include "Game.h"
#include "../Renderer/Renderer.h"
#include "../Assets/AssetManager.h"
#include "../Scene/Scene.h"

namespace Game {
    void Init() {
        int blankIdx = Renderer::LoadShader("shaders/blank.vert", "shaders/blank.frag");
        int phongIdx = Renderer::LoadShader("shaders/phong.vert", "shaders/phong.frag");

        // int sponzaIdx = AssetManager::LoadModel("res/sponza/sponza.obj");
        int cubeIdx = AssetManager::LoadModel("res/cube/cube.obj");

        // SceneObject sponza;
        // sponza.m_modelIndex = sponzaIdx;
        // sponza.m_shaderIndex = phongIdx;
        // sponza.m_scale = glm::vec3(0.1f);
        // Scene::g_sceneObjects.push_back(sponza);

        SceneObject lightCube;
        lightCube.m_modelIndex = cubeIdx;
        lightCube.m_shaderIndex = blankIdx;
        lightCube.m_scale = glm::vec3(0.25);
        lightCube.m_position = glm::vec3(0, 0, 3);
        Scene::g_sceneObjects.push_back(lightCube);

        SceneObject brick; 
        brick.m_modelIndex = cubeIdx;
        brick.m_shaderIndex = phongIdx;
        brick.m_position = glm::vec3(0, 0, 0);
        Material& mat = AssetManager::GetModelByIndex(cubeIdx)->GetMaterials()[0];
        mat.m_diffuseMap.Load(("res/textures/red_brick/red_brick_diff_4k.jpg"));
        mat.m_normalMap.Load("res/textures/red_brick/red_brick_nor_gl_4k.png");
        Scene::g_sceneObjects.push_back(brick);

        Scene::g_camera = Camera(glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}
