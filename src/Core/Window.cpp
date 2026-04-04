#include "Window.h"
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

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    glfwSwapInterval(0);
}

void Window::BeginFrame() {
    glfwPollEvents();
}

void Window::EndFrame() {
    glfwSwapBuffers(m_window);
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(m_window);
}
