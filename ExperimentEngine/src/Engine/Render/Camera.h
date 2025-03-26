#pragma once

namespace Exp
{
    class Camera
    {
    public:
        Camera();
        Camera(float fov, float aspectRatio, float near, float far);

        inline const glm::mat4& GetProjection() const { return m_Projection; }
        
    private:
        bool OnWindowResized(const WindowResizeEvent& e);
        
        void RecalculateProjection();
        
        glm::mat4 m_Projection = glm::mat4(1.f);

        float m_FOV = 45.f;
        float m_Near = .01f, m_Far = 1000.f;

        float m_AspectRatio = 16.f / 9.f;
        
    };
}
