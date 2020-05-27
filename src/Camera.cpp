#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH = 0.0f;
const float DEFAULT_SPEED = 2.5f;
const float DEFAULT_SENSITIVITY = 0.1f;
const float DEFAULT_ZOOM = 45.0f;

Camera::Camera(const glm::vec3& initialPosition) : m_position { initialPosition } {
    m_yaw = DEFAULT_YAW;
    m_pitch = DEFAULT_PITCH;
    m_movementSpeed = DEFAULT_SPEED;
    m_mouseSensitivity = DEFAULT_SENSITIVITY;
    m_zoom = DEFAULT_ZOOM;
    updateCamera();
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    switch (direction) {
        case FORWARD:  m_position += m_forward * velocity; break;
        case BACKWARD: m_position -= m_forward * velocity; break;
        case LEFT:     m_position -= m_right * velocity;   break;
        case RIGHT:    m_position += m_right * velocity;   break;
    }
}

void Camera::processMouseMovement(float mouseX, float mouseY) {
    static float lastMouseX = mouseX;
    static float lastMouseY = mouseY;
    m_yaw += (mouseX - lastMouseX) * m_mouseSensitivity;
    m_pitch += (lastMouseY - mouseY) * m_mouseSensitivity;
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    m_pitch = std::clamp(m_pitch, -89.5f, 89.5f);
    updateCamera();
}

void Camera::processMouseScroll(float offsetY) {
    m_zoom -= offsetY;
    m_zoom = std::clamp(m_zoom, 1.0f, 45.0f);
}

glm::vec3 Camera::getCameraPosition() const {
    return m_position;
}

float Camera::getZoom() const {
    return m_zoom;
}

void Camera::updateCamera() {
    glm::vec3 newForward;
    newForward.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    newForward.y = std::sin(glm::radians(m_pitch));
    newForward.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_forward = glm::normalize(newForward);
    m_right = glm::normalize(glm::cross(m_forward, WORLD_UP));
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}