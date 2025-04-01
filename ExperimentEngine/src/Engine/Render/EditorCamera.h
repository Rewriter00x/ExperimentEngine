#pragma once

#include "Camera.h"

namespace Exp
{
    class EditorCamera : public Camera
    {
    public:
        EditorCamera();
        
        void OnUpdate(float deltaSeconds);
        
        void AddMovementInput(const glm::vec3& input);
        void AddRotationInput(const glm::vec3& input);
        void AddMovementAndRotationInput(const glm::vec3& movementInput, const glm::vec3& rotationInput);

    private:
        bool OnMouseButtonPressed(const MouseButtonPressedEvent& e);
        bool OnMouseButtonReleased(const MouseButtonReleasedEvent& e);

        glm::vec2 m_LastMousePos = glm::vec2(0.f);
        
        float m_CameraMoveSpeed = 10.f;
        float m_CameraRotationSpeed = 45.f;
        
    };
}
