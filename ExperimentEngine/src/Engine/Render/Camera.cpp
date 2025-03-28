#include "exppch.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Exp
{
    static float s_CameraMoveSpeed = 10.f;
    static float s_CameraRotationSpeed = 45.f;
    
    Camera::Camera()
    {
        RecalculateProjection();
    }

    Camera::Camera(float fov, float aspectRatio, float near, float far)
        : m_FOV(fov), m_Near(near), m_Far(far), m_AspectRatio(aspectRatio)
    {
        RecalculateProjection();
    }

    void Camera::AddMovementInput(const glm::vec3& input)
    {
        const glm::vec3 transformedInput = GetRotationQuat() * input;
        SetPosition(m_Position + transformedInput * s_CameraMoveSpeed);
    }

    void Camera::AddRotationInput(const glm::vec3& input)
    {
        SetRotation(m_Rotation + input * s_CameraRotationSpeed);
    }

    void Camera::AddMovementAndRotationInput(const glm::vec3& movementInput, const glm::vec3& rotationInput)
    {
        const glm::vec3 transformedMovementInput = GetRotationQuat() * movementInput;
        SetPositionAndRotation(m_Position + transformedMovementInput * s_CameraMoveSpeed, m_Rotation + rotationInput * s_CameraRotationSpeed);
    }

    void Camera::RecalculateProjection()
    {
        m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
    }

    void Camera::RecalculateView()
    {
        m_View = glm::inverse(glm::translate(glm::mat4(1.f), m_Position) * glm::toMat4(GetRotationQuat()));
    }
}
