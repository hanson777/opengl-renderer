#include <string>
#include <glad/glad.h>

class Texture {
  public:
    Texture(std::string filename);

    uint32_t id() const { return m_id; }

  private:
    uint32_t m_id; 

    void loadTextureImage(std::string filename);
    void setTexture(uint8_t* data, GLenum format, int width, int height);
};
