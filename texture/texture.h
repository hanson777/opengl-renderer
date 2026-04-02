#pragma once
#include <string>
#include <glad/glad.h>

class Texture {
  public:
    GLuint Id() const { return m_id; }
    void Load(const std::string filename);

  private:
    GLuint m_id = 0;

    void BindTexture(uint8_t* data, GLenum format, int width, int height);
};
