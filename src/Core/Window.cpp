#include "Window.h"
#include "../Input/Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(int width, int height, const char* title) : m_width(width), m_height(height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        std::cout << "[ERROR::WINDOW] failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "[ERROR::WINDOW] failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
    glfwSetCursorPosCallback(m_window, MouseCallback);
    glfwSetScrollCallback(m_window, ScrollCallback);

    Input::SetWindow(m_window);

    glfwMaximizeWindow(m_window);
    glfwSwapInterval(0);
}

void Window::BeginFrame() {
    glfwPollEvents();
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(m_window, true);
}

void Window::EndFrame() {
    glfwSwapBuffers(m_window);
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(m_window);
}

void Window::Shutdown() {
    glfwTerminate();
}

void Window::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Input::UpdateMouse(xpos, ypos);
}

void Window::ScrollCallback(GLFWwindow* window, double dx, double dy) {
    Input::UpdateScroll(dy);
}