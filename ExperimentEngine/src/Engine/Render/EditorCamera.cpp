#include "exppch.h"
#include "EditorCamera.h"

namespace Exp
{
    EditorCamera::EditorCamera()
    {
        ADD_EVENT_LISTENER(this, MouseButtonPressed, OnMouseButtonPressed);
        ADD_EVENT_LISTENER(this, MouseButtonReleased, OnMouseButtonReleased);
    }

    void EditorCamera::OnUpdate(float deltaSeconds)
    {
        glm::vec3 movementInput(0.f);
        if (Input::IsKeyPressed(KeyCode::A))	movementInput.x = -1.f;
        if (Input::IsKeyPressed(KeyCode::D))	movementInput.x = 1.f;
        if (Input::IsKeyPressed(KeyCode::S))	movementInput.z = 1.f;
        if (Input::IsKeyPressed(KeyCode::W))	movementInput.z = -1.f;

        glm::vec3 rotationInput(0.f);
        if (Input::IsKeyPressed(KeyCode::E))	rotationInput.z = -1.f;
        if (Input::IsKeyPressed(KeyCode::Q))	rotationInput.z = 1.f;
        if (Input::IsMouseButtonPressed(MouseCode::ButtonRight))
        {
            const glm::vec2 mousePos = Input::GetMousePosition();
            const glm::vec2 delta = m_LastMousePos - mousePos;
            rotationInput.x = delta.y;
            rotationInput.y = delta.x;
            m_LastMousePos = mousePos;
        }
        
        if (movementInput != glm::vec3(0.f) || rotationInput != glm::vec3(0.f))
        {
            AddMovementAndRotationInput(movementInput * deltaSeconds, rotationInput * deltaSeconds);
        }
    }

    bool EditorCamera::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == MouseCode::ButtonRight)
        {
            m_LastMousePos = Input::GetMousePosition();
            return true;
        }
        return false;
    }

    bool EditorCamera::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
    {
        if (e.GetMouseButton() == MouseCode::ButtonRight)
        {
            m_LastMousePos = glm::vec2(0.f);
            return true;
        }
        return false;
    }
}
