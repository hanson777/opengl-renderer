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
        lightCube.m_scale = glm::vec3(0.25);
        lightCube.m_position = glm::vec3(0, 3, 4);

        // Light light;
        // light.type = LightType::Directional;
        // light.color = glm::vec3(0, 1, 0);
        // light.direction = glm::vec3(0, -1, 0);
        // light.intensity = 0.3f;
        // Scene::g_lights.push_back(light);
        
        Light pointLight;
        pointLight.type = LightType::Point;
        pointLight.color = glm::vec3(1, 1, 1);
        pointLight.position = lightCube.m_position; 
        pointLight.intensity = 0.2f;

        for (int i = 0; i < 3; i++) {
            pointLight.position.x = i*5;
            Scene::g_lights.push_back(pointLight);
            lightCube.m_position.x = i*5;
            Scene::g_sceneObjects.push_back(lightCube);
        }
        
        SceneObject brick;
        brick.m_modelIndex = cubeIdx;
        brick.m_shaderIndex = phongIdx;
        brick.m_position = glm::vec3(0, 3, 0);
        Material& mat = AssetManager::GetModelByIndex(cubeIdx)->GetMaterials()[0];
        mat.m_diffuseMap.Load("res/textures/red_brick/red_brick_diff_4k.jpg");
        mat.m_normalMap.Load("res/textures/red_brick/red_brick_nor_gl_4k.png");

        for (int i = 0; i < 10; i++) {
            brick.m_position = glm::vec3(i*3, 3, 0);
            Scene::g_sceneObjects.push_back(brick);
        }

        Scene::g_camera = Camera(glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}
