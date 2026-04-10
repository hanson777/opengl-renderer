#include "Game.h"
#include "../Renderer/Renderer.h"
#include "../Assets/AssetManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Light.h"

namespace Game {
    void Init() {
        int blankIdx = Renderer::LoadShader("shaders/blank.vert", "shaders/blank.frag");
        int phongIdx = Renderer::LoadShader("shaders/phong.vert", "shaders/phong.frag");
        int redIdx = Renderer::LoadShader("shaders/blank.vert", "shaders/red.frag");
        int greenIdx = Renderer::LoadShader("shaders/blank.vert", "shaders/green.frag");
        int blueIdx = Renderer::LoadShader("shaders/blank.vert", "shaders/blue.frag");

        int sponzaIdx = AssetManager::LoadModel("res/sponza/sponza.obj");
        SceneObject sponza;
        sponza.m_modelIndex = sponzaIdx;
        sponza.m_shaderIndex = phongIdx;
        sponza.m_scale = glm::vec3(0.1f);
        Scene::g_sceneObjects.push_back(sponza);

        int cubeIdx = AssetManager::LoadModel("res/cube/cube.obj");

        SceneObject lightCube;
        lightCube.m_modelIndex = cubeIdx;
        lightCube.m_shaderIndex = blankIdx;
        lightCube.m_scale = glm::vec3(0.25);
        lightCube.m_position = glm::vec3(0, 3, 4);

        // Light dirLight;
        // dirLight.type = LightType::Directional;
        // dirLight.color = glm::vec3(1, 1, 1);
        // dirLight.direction = glm::vec3(0, -1, 0);
        // dirLight.intensity = 0.3f;
        // Scene::g_lights.push_back(dirLight);
        
        Light pointLight;
        pointLight.type = LightType::Point;
        pointLight.color = glm::vec3(1, 0, 0);
        pointLight.position = lightCube.m_position; 
        pointLight.intensity = 0.2f;
        pointLight.radius = 10;

        SceneObject brick;
        brick.m_modelIndex = cubeIdx;
        brick.m_shaderIndex = phongIdx;
        brick.m_position = lightCube.m_position;
        brick.m_position.z = lightCube.m_position.z + 5;
        Material& mat = AssetManager::GetModelByIndex(cubeIdx)->GetMaterials()[0];
        mat.m_diffuseMap.Load("res/textures/red_brick/red_brick_diff_4k.jpg");
        mat.m_normalMap.Load("res/textures/red_brick/red_brick_nor_gl_4k.png");

        for (int i = 0; i < 10; i++) {
            brick.m_position.x = i*15;
            Scene::g_sceneObjects.push_back(brick);
            pointLight.position.x = i*15;
            if (i % 3 == 0) { pointLight.color = glm::vec3(1, 0, 0); lightCube.m_shaderIndex = redIdx; }
            if (i % 3 == 1) { pointLight.color = glm::vec3(0, 1, 0); lightCube.m_shaderIndex = greenIdx; }
            if (i % 3 == 2) { pointLight.color = glm::vec3(0, 0, 1); lightCube.m_shaderIndex = blueIdx; }
            Scene::g_lights.push_back(pointLight);
            lightCube.m_position.x = i*15;
            Scene::g_sceneObjects.push_back(lightCube);
        }
        
        Scene::g_camera = Camera(glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}
