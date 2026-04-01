#include <string>
#include <glad/glad.h>

class Texture {
  public:
    uint32_t Id() const { return m_id; }
    uint32_t Width() const { return m_width; }
    uint32_t Height() const { return m_height; }
    void Load(const std::string filename);

  private:
    uint32_t m_id, m_width, m_height;

    void BindTexture(uint8_t* data, GLenum format, int width, int height);
};
