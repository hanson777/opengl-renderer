#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Input {
    static GLFWwindow* s_window = nullptr;
    static glm::vec2 s_mouseDelta(0.0f);
    static float s_scrollDelta = 0.0f;
    static bool s_firstMouse = true;
    static float s_lastX = 0.0f;
    static float s_lastY = 0.0f;

    void SetWindow(GLFWwindow* window) { s_window = window; }

    void Update() {
        s_mouseDelta = glm::vec2(0.0f);
        s_scrollDelta = 0.0f;
    }

    bool KeyPressed(int key) {
        return glfwGetKey(s_window, key) == GLFW_PRESS;
    }

    glm::vec2 GetMouseDelta() { return s_mouseDelta; }

    float GetScrollDelta() { return s_scrollDelta; }

    void UpdateMouse(double xpos, double ypos) {
        if (s_firstMouse) {
            s_lastX = xpos;
            s_lastY = ypos;
            s_firstMouse = false;
        }

        s_mouseDelta.x += xpos - s_lastX;
        s_mouseDelta.y += s_lastY - ypos;

        s_lastX = xpos;
        s_lastY = ypos;
    }

    void UpdateScroll(double dy) {
        s_scrollDelta = dy;
    }
}
