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
        sponza.modelIndex = sponzaIdx;
        sponza.shaderIndex = phongIdx;
        sponza.scale = glm::vec3(0.1f);
        Scene::g_sceneObjects.push_back(sponza);

        int cubeIdx = AssetManager::LoadModel("res/cube/cube.obj");

        SceneObject lightCube;
        lightCube.modelIndex = cubeIdx;
        lightCube.shaderIndex = blankIdx;
        lightCube.scale = glm::vec3(0.25);
        lightCube.position = glm::vec3(0, 3, 4);
        Scene::g_sceneObjects.push_back(lightCube);

        Light pointLight;
        pointLight.type = LightType::Point;
        pointLight.color = glm::vec3(1, 0, 0);
        pointLight.position = lightCube.position;
        pointLight.intensity = 0.2f;
        pointLight.radius = 10;
        Scene::g_lights.push_back(pointLight);

        Scene::g_camera = Camera(glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void InitPrimitives() {
        int ndcIdx = Renderer::LoadShader("shaders/quad.vert", "shaders/quad.frag");
        // int ndcIdx = Renderer::LoadShader("Shaders/ndc.vert", "shaders/ndc.frag");

        int quadIdx = AssetManager::LoadMeshData(MeshType::Quad);

        ScreenSpaceObject screenSpaceQuad;
        screenSpaceQuad.meshIndex = quadIdx;
        screenSpaceQuad.shaderIndex = ndcIdx;
        Scene::g_screenSpaceObjects.push_back(screenSpaceQuad);
    }
}
