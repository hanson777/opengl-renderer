#include "window.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

Window::Window(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cout << "[ERROR::WINDOW] failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    m_window = window;

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "[ERROR::WINDOW] failed to initialize GLAD" << std::endl;
    }

    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
}

void Window::beginFrame() {
    glfwPollEvents();
}

void Window::endFrame() {
    glfwSwapBuffers(m_window);
}
