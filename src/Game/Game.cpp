#include "Game.h"
#include "../Renderer/Renderer.h"
#include "../Assets/AssetManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Light.h"
#include <array>
#include <cstddef>

namespace Game {
    void Init() {
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

        const std::array<glm::vec3, 6> lightPositions = {{
            {-70.0f, 8.0f, -18.0f},
            {-42.0f, 10.0f, 18.0f},
            {-14.0f, 8.0f, -18.0f},
            {14.0f, 10.0f, 18.0f},
            {42.0f, 8.0f, -18.0f},
            {70.0f, 10.0f, 18.0f},
        }};
        const std::array<glm::vec3, 3> lightColors = {{
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
        }};
        const std::array<int, 3> lightShaders = {redIdx, greenIdx, blueIdx};

        for (size_t i = 0; i < lightPositions.size(); ++i) {
            const size_t colorIndex = i % lightColors.size();

            SceneObject lightCube;
            lightCube.modelIndex = cubeIdx;
            lightCube.shaderIndex = lightShaders[colorIndex];
            lightCube.scale = glm::vec3(0.75f);
            lightCube.position = lightPositions[i];
            Scene::g_sceneObjects.push_back(lightCube);

            Light pointLight{};
            pointLight.type = LightType::Point;
            pointLight.color = lightColors[colorIndex];
            pointLight.position = lightCube.position;
            pointLight.intensity = 2.0f;
            pointLight.radius = 55.0f;
            Scene::g_lights.push_back(pointLight);
        }

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
