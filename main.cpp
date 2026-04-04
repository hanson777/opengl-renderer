#include "src/Core/Window.h"
#include "src/Input/Input.h"
#include "src/Renderer/Renderer.h"
#include "src/Scene/Scene.h"
#include "src/Game/Game.h"

int main() {
    Window window(1920, 1080, "hi");

    Renderer::Init();
    Game::Init();
    Renderer::Upload();

    while (!window.ShouldClose()) {
        window.BeginFrame();

        Scene::Update(Renderer::g_deltaTime);
        Input::Update();
        Renderer::RenderFrame();

        window.EndFrame();
    }

    window.Shutdown();
    return 0;
}
