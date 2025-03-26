#pragma once

namespace Exp
{
    namespace Input
    {
        bool IsKeyPressed(KeyCode key);

        bool IsMouseButtonPressed(MouseCode button);
        glm::vec2 GetMousePosition();
        float GetMouseX();
        float GetMouseY();

    }
}
