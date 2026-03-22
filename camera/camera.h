#include <glm/glm.hpp>

class Camera {
  public:
    enum Direction { Forward, Backward, Left, Right };
    Camera(glm::vec3 pos, glm::vec3 up);
    void processKeyboard(Direction d, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    glm::mat4 view();
  private:
    void updateCameraVectors();
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
