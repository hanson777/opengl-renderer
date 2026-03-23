#include "camera.h"
#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM =  45.0f;

Camera::Camera(glm::vec3 pos, glm::vec3 up) {
    m_pos = pos;
    m_up = up;
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_flatFront = m_front;
    m_yaw = YAW;
    m_pitch = PITCH;
    m_moveSpeed = SPEED;
    m_mouseSensitivity = SENSITIVITY;
    m_zoom = ZOOM;
    updateCameraVectors();
}

void Camera::processKeyboard(Direction d, float deltaTime) {
        float velocity = m_moveSpeed * deltaTime;
        if (d == Forward)
            m_pos += m_flatFront * velocity;
        if (d == Backward)
            m_pos -= m_flatFront * velocity;
        if (d == Left)
            m_pos -= m_right * velocity;
        if (d == Right)
            m_pos += m_right * velocity;
        if (d == Up) 
            m_pos.y += velocity;
        if (d == Down)
            m_pos.y -= velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw   += xoffset;
        m_pitch += yoffset;

        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

        updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
        m_zoom -= yoffset;
        m_zoom = std::clamp(m_zoom, 1.0f, 45.0f);
    }

glm::mat4 Camera::view() {
    return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

void Camera::updateCameraVectors() {
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);
    m_flatFront = glm::vec3(m_front.x, 0.0f, m_front.z);
    m_right = glm::normalize(glm::cross(m_front, m_up));
    // m_up = glm::normalize(glm::cross(m_right, m_front));
}
