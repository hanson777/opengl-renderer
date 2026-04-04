#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

namespace Input {
    void SetWindow(GLFWwindow* window);
    void Update();

    bool KeyPressed(int key);
    glm::vec2 GetMouseDelta();
    float GetScrollDelta();

    void UpdateMouse(double xpos, double ypos);
    void UpdateScroll(double dy);
}
