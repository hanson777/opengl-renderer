#include "Game.h"
#include "../Renderer/Renderer.h"
#include "../Assets/AssetManager.h"
#include "../Assets/Primitive.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Light.h"
#include <iostream>

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

        Scene::g_camera = Camera(glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void InitPrimitives() {
        int ndcIdx = Renderer::LoadShader("shaders/quad.vert", "shaders/quad.frag");

        int quadIdx = AssetManager::LoadPrimitive(PrimitiveType::Quad);

        ScreenSpaceObject screenSpaceQuad;
        screenSpaceQuad.primitiveIndex = quadIdx;
        screenSpaceQuad.shaderIndex = ndcIdx;
        Scene::g_screenSpaceObjects.push_back(screenSpaceQuad);
    }
}