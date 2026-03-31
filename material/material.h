#include <string>

class Material {
  public:
    void bind();

  private:
    std::string m_name;
    uint32_t m_alb;
    uint32_t m_nrm;
};
