#include "exppch.h"
#include "Camera.h"

namespace Exp
{
    Camera::Camera(float fov, float aspectRatio, float near, float far)
        : m_ProjectionComponent(fov, aspectRatio, near, far)
    {
    }

    void Camera::RecalculateView()
    {
        m_View = glm::inverse(glm::translate(glm::mat4(1.f), m_Position) * glm::toMat4(GetRotationQuat()));
    }
}
