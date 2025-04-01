#pragma once

namespace Exp
{
    class Camera
    {
    public:
        Camera();
        Camera(float fov, float aspectRatio, float near, float far);

        inline const glm::mat4& GetProjection() const { return m_Projection; }
        inline const glm::mat4& GetView() const { return m_View; }

        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline const glm::vec3& GetRotation() const { return m_Rotation; }
        inline glm::quat GetRotationQuat() const { return glm::quat(glm::radians(m_Rotation)); }
        
        inline glm::mat4 GetViewProjection() const { return m_Projection * m_View; }

        inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; RecalculateProjection(); }
        
        inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateView(); }
        inline void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateView(); }
        inline void SetPositionAndRotation(const glm::vec3& position, const glm::vec3& rotation) { m_Position = position; m_Rotation = rotation; RecalculateView(); }
        
    private:
        void RecalculateProjection();
        void RecalculateView();
        
        glm::mat4 m_Projection = glm::mat4(1.f);
        glm::mat4 m_View = glm::mat4(1.f);

        glm::vec3 m_Position = glm::vec3(0.f);
        glm::vec3 m_Rotation = glm::vec3(0.f);

        float m_FOV = 45.f;
        float m_Near = .01f, m_Far = 1000.f;

        float m_AspectRatio = 16.f / 9.f;
        
    };
}
