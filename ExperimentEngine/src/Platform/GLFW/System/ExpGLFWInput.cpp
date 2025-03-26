#include "exppch.h"
#include "Engine/System/Input.h"

#include "GLFW/glfw3.h"

namespace Exp::Input
{
    bool IsKeyPressed(KeyCode key)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        const int state = glfwGetKey(window, (int)key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool IsMouseButtonPressed(MouseCode button)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        const int state = glfwGetMouseButton(window, (int)button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    glm::vec2 GetMousePosition()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return { (float)xPos, (float)yPos };
    }

    float GetMouseX()
    {
        return GetMousePosition().x;
    }

    float GetMouseY()
    {
        return GetMousePosition().y;
    }
}