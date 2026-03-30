#include <string>
#include <glad/glad.h>

class Material {
  public:
    void bind();

  private:
    std::string m_name;
    GLuint m_alb;
    GLuint m_nrm;
};
