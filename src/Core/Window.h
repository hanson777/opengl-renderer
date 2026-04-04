#pragma once

struct GLFWwindow;

class Window {
  public:
    Window(int width, int height, const char* title);
    void BeginFrame();
    void EndFrame();
    bool ShouldClose();
    void Shutdown();
    GLFWwindow* GetHandle() const { return m_window; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

  private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
