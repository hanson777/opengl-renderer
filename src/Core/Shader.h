#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(std::string vertexPath, std::string fragmentPath);

    void Use();
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat4) const;
    uint32_t GetId() const;

private:
    uint32_t m_id;
};
