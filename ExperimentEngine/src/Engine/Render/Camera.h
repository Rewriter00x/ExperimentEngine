#pragma once

#include "Engine/ECS/Components/CameraComponent.h"

namespace Exp
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(float fov, float aspectRatio, float near, float far);

        inline const glm::mat4& GetProjection() const { return m_ProjectionComponent.GetProjection(); }
        inline const glm::mat4& GetView() const { return m_View; }

        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline const glm::vec3& GetRotation() const { return m_Rotation; }
        inline glm::quat GetRotationQuat() const { return glm::quat(glm::radians(m_Rotation)); }
        
        inline glm::mat4 GetViewProjection() const { return m_ProjectionComponent.GetProjection() * m_View; }

        inline void SetAspectRatio(float aspectRatio) { m_ProjectionComponent.SetAspectRatio(aspectRatio); }
        
        inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateView(); }
        inline void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateView(); }
        inline void SetPositionAndRotation(const glm::vec3& position, const glm::vec3& rotation) { m_Position = position; m_Rotation = rotation; RecalculateView(); }
        
    private:
        void RecalculateView();
        
        CameraComponent m_ProjectionComponent;
        
        glm::mat4 m_View = glm::mat4(1.f);

        glm::vec3 m_Position = glm::vec3(0.f);
        glm::vec3 m_Rotation = glm::vec3(0.f);
        
    };
}
