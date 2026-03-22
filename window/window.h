#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(int width, int height, const char* title);
    void beginFrame();
    void endFrame();
    GLFWwindow* window(); 
    int height() const;
    int width() const;
  private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    bool m_shouldClose;
};
