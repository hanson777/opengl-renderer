#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
  public:
    Shader(std::string vertexPath, std::string fragmentPath);
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat4) const;
    uint32_t id() const;

  private:
    uint32_t m_id;
};
