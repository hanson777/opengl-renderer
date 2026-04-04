#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(int width, int height, const char* title);
    void BeginFrame();
    void EndFrame();
    bool ShouldClose();
    GLFWwindow* GetHandle() { return m_window; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

  private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
};
