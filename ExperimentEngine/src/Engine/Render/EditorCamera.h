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

        inline void SetShouldCaptureMouse(bool shouldCaptureMouse) { m_ShouldCaptureMouse = shouldCaptureMouse; }
        inline void SetShouldCaptureKey(bool shouldCaptureKey) { m_ShouldCaptureKey = shouldCaptureKey; }

    private:
        bool OnMouseButtonPressed(const MouseButtonPressedEvent& e);
        bool OnMouseButtonReleased(const MouseButtonReleasedEvent& e);

        glm::vec2 m_LastMousePos = glm::vec2(0.f);
        
        float m_CameraMoveSpeed = 10.f;
        float m_CameraRotationSpeed = 45.f;

        bool m_ShouldCaptureMouse = false;
        bool m_ShouldCaptureKey = false;

        bool m_IsCapturingMouse = false;
        
    };
}
