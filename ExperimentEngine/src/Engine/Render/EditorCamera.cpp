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
        if (m_ShouldCaptureKey)
        {
            if (Input::IsKeyPressed(KeyCode::A))	movementInput.x = -1.f;
            if (Input::IsKeyPressed(KeyCode::D))	movementInput.x = 1.f;
            if (Input::IsKeyPressed(KeyCode::S))	movementInput.z = 1.f;
            if (Input::IsKeyPressed(KeyCode::W))	movementInput.z = -1.f;
        }

        glm::vec3 rotationInput(0.f);
        if (m_ShouldCaptureKey)
        {
            if (Input::IsKeyPressed(KeyCode::E))	rotationInput.z = -1.f;
            if (Input::IsKeyPressed(KeyCode::Q))	rotationInput.z = 1.f;
            if (m_IsCapturingMouse && Input::IsMouseButtonPressed(MouseCode::ButtonRight))
            {
                const glm::vec2 mousePos = Input::GetMousePosition();
                const glm::vec2 delta = m_LastMousePos - mousePos;
                rotationInput.x = delta.y;
                rotationInput.y = delta.x;
                m_LastMousePos = mousePos;
            }
        }
        if (movementInput != glm::vec3(0.f) || rotationInput != glm::vec3(0.f))
        {
            AddMovementAndRotationInput(movementInput * deltaSeconds, rotationInput * deltaSeconds);
        }
    }

    void EditorCamera::AddMovementInput(const glm::vec3& input)
    {
        const glm::vec3 transformedInput = GetRotationQuat() * input;
        SetPosition(GetPosition() + transformedInput * m_CameraMoveSpeed);
    }

    void EditorCamera::AddRotationInput(const glm::vec3& input)
    {
        SetRotation(GetRotation() + input * m_CameraRotationSpeed);
    }

    void EditorCamera::AddMovementAndRotationInput(const glm::vec3& movementInput, const glm::vec3& rotationInput)
    {
        const glm::vec3 transformedMovementInput = GetRotationQuat() * movementInput;
        SetPositionAndRotation(GetPosition() + transformedMovementInput * m_CameraMoveSpeed, GetRotation() + rotationInput * m_CameraRotationSpeed);
    }

    bool EditorCamera::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
    {
        if (m_ShouldCaptureMouse && e.GetMouseButton() == MouseCode::ButtonRight)
        {
            m_IsCapturingMouse = true;
            m_LastMousePos = Input::GetMousePosition();
            return true;
        }
        return false;
    }

    bool EditorCamera::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
    {
        if (e.GetMouseButton() == MouseCode::ButtonRight)
        {
            const bool wasCapturingMouse = m_IsCapturingMouse;
            m_IsCapturingMouse = false;
            m_LastMousePos = glm::vec2(0.f);
            return wasCapturingMouse;
        }
        return false;
    }
}
