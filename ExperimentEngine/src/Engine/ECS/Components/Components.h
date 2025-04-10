#pragma once

#include "imgui.h"

#include "SpriteComponent.h"

namespace Exp
{
    template<typename... Components>
    struct ComponentList {};

    template<typename T>
    const char* GetComponentName();

    template<typename T>
    void DrawComponent(Entity& e);
    
    // BEGIN TODO REFLECTOR
    using AllComponents = ComponentList<SpriteComponent>;
    
    template<>
    inline const char* GetComponentName<SpriteComponent>()
    {
        return "Sprite Component";
    }
    
    template<>
    inline void DrawComponent<SpriteComponent>(Entity& e)
    {
        SpriteComponent& component = e.GetComponent<SpriteComponent>();
        ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
        ImGui::Button("Texture", { 100.f, 0.f });
    }
    // END TODO REFLECTOR
}