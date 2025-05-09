#include "exppch.h"
#include "CameraComponent.h"

namespace Exp
{
    CameraComponent::CameraComponent()
    {
        RecalculateProjection();
    }

    CameraComponent::CameraComponent(float fov, float aspectRatio, float near, float far)
        : FOV(fov), Near(near), Far(far), AspectRatio(aspectRatio)
    {
        RecalculateProjection();
    }

    void CameraComponent::RecalculateProjection()
    {
        Projection = glm::perspective(glm::radians(FOV), AspectRatio, Near, Far);
    }
}
