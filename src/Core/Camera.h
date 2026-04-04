#pragma once
#include <glm/glm.hpp>

class Camera {
  public:
    Camera(glm::vec3 pos, glm::vec3 up);
    enum Direction { Forward, Backward, Left, Right, Up, Down};

    void Update(float deltaTime);
    void ProcessKeyboard(Direction d, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);
    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPosition() const { return m_pos; }
    float GetFov() const { return m_zoom; }

  private:
    void UpdateCameraVectors();

    glm::vec3 m_pos;
    glm::vec3 m_up;
    glm::vec3 m_front;
    glm::vec3 m_flatFront;
    glm::vec3 m_right;
    float m_yaw;
    float m_pitch;
    float m_moveSpeed;
    float m_mouseSensitivity;
    float m_zoom;
};
