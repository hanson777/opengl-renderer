#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Input {
    void SetWindow(GLFWwindow* window);
    void Update();

    bool KeyPressed(int key);
    glm::vec2 GetMouseDelta();
    float GetScrollDelta();

    void UpdateMouse(double xpos, double ypos);
    void UpdateScroll(double dy);
}
