#pragma once

#include "Camera.h"

namespace Exp
{
    class EditorCamera : public Camera
    {
    public:
        EditorCamera();
        
        void OnUpdate(float deltaSeconds);

    private:
        bool OnMouseButtonPressed(const MouseButtonPressedEvent& e);
        bool OnMouseButtonReleased(const MouseButtonReleasedEvent& e);

        glm::vec2 m_LastMousePos = glm::vec2(0.f);
        
    };
}
