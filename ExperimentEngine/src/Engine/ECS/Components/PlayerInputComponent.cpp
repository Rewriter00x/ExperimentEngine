#include "exppch.h"
#include "PlayerInputComponent.h"

#include "SpriteComponent.h"

namespace Exp
{
    void PlayerInputComponent::Start()
    {
        ADD_EVENT_LISTENER(this, KeyPressed, OnKeyPressed);
    }

    void PlayerInputComponent::End()
    {
        REMOVE_EVENT_LISTENER(this, KeyPressed);
    }

    void PlayerInputComponent::OnUpdate(float deltaSeconds)
    {
        glm::vec3 movementInput = glm::vec3(0.f);

        if (Input::IsKeyPressed(KeyCode::Left))	movementInput.x = -1.f;
        if (Input::IsKeyPressed(KeyCode::Right))	movementInput.x = 1.f;
        if (Input::IsKeyPressed(KeyCode::Down))	movementInput.y = -1.f;
        if (Input::IsKeyPressed(KeyCode::Up))	movementInput.y = 1.f;
        
        GetEntity().AddMovementInput(movementInput, MoveSpeed * deltaSeconds);
    }

    bool PlayerInputComponent::OnKeyPressed(const KeyPressedEvent& e)
    {
        Entity& entity = GetEntity();
        if (e.GetKeyCode() == KeyCode::Space)
        {
            if (entity.HasComponent<SpriteComponent>())
            {
                glm::vec4 color = entity.GetComponent<SpriteComponent>().Color;
                color = glm::vec4(1.f) - color;
                entity.GetComponent<SpriteComponent>().Color = color;
            }
            return true;
        }
        return false;
    }
}
