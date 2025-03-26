#include "exppch.h"
#include "Camera.h"

namespace Exp
{
    Camera::Camera()
    {
        RecalculateProjection();
        ADD_EVENT_LISTENER(this, WindowResize, OnWindowResized);
    }

    Camera::Camera(float fov, float aspectRatio, float near, float far)
        : m_FOV(fov), m_Near(near), m_Far(far), m_AspectRatio(aspectRatio)
    {
        RecalculateProjection();
    }

    bool Camera::OnWindowResized(const WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        RecalculateProjection();
        return false;
    }

    void Camera::RecalculateProjection()
    {
        m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
    }
}
