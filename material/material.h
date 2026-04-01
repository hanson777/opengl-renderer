#include <string>

class Material {
  public:
    void Bind();

  private:
    std::string m_name;
    uint32_t m_alb;
    uint32_t m_nrm;
};
