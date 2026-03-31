#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(int width, int height, const char* title);
    void beginFrame();
    void endFrame();
    GLFWwindow* window() { return m_window; };

  private:
    GLFWwindow* m_window;
};
