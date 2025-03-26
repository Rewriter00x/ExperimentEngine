#include "exppch.h"
#include "Camera.h"

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
}
