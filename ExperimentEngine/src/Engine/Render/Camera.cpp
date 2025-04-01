#include "exppch.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Exp
{
    Camera::Camera()
    {
        RecalculateProjection();
    }

    Camera::Camera(float fov, float aspectRatio, float near, float far)
        : m_FOV(fov), m_Near(near), m_Far(far), m_AspectRatio(aspectRatio)
    {
        RecalculateProjection();
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
