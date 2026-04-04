#include "src/Core/Window.h"
#include "src/Renderer/Renderer.h"
#include "src/Scene/Scene.h"

int main() {
    Window window(1920, 1080, "hi");

    Renderer::Init();
    Scene::Init();
    Renderer::Upload();

    while (!window.ShouldClose()) {
        window.BeginFrame();

        Renderer::RenderFrame();

        window.EndFrame();
    }

    window.Shutdown();
    return 0;
}
