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

    Camera::Camera(const glm::vec3& position, const glm::vec3& rotation,
                   float fov, float aspectRatio, float near, float far)
        : m_Position(position), m_Rotation(rotation)
        , m_FOV(fov), m_Near(near), m_Far(far), m_AspectRatio(aspectRatio)
    {
        RecalculateView();
        RecalculateProjection();
    }

    Camera::Camera(const glm::mat4& projection, const glm::mat4& view)
        : m_Projection(projection), m_View(view)
    {
        // don't recalculate here, since this constructor is for plain old data
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
