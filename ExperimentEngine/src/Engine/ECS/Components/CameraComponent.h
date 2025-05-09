#pragma once

namespace Exp
{
    //c
    struct CameraComponent
    {
        glm::mat4 Projection = glm::mat4(1.f);
        
        //p draw, save, onEdit = RecalculateProjection
        float FOV = 45.f;
        
        //p draw, save, onEdit = RecalculateProjection
        float Near = .01f;
        
        //p draw, save, onEdit = RecalculateProjection
        float Far = 1000.f;

        //p draw, save, onEdit = RecalculateProjection
        float AspectRatio = 16.f / 9.f;

        CameraComponent();
        CameraComponent(float fov, float aspectRatio, float near, float far);
        
        inline const glm::mat4& GetProjection() const { return Projection; }
        
        inline void SetAspectRatio(float aspectRatio) { AspectRatio = aspectRatio; RecalculateProjection(); }
        
        void RecalculateProjection();
        
    };
}
