#include "exppch.h"
#include "ComponentUtils.h"

#include "imgui.h"
#include "yaml-cpp/yaml.h"

#include "SpriteComponent.h"

namespace Exp
{
    // BEGIN TODO REFLECTOR
    template<>
    const char* GetComponentName<SpriteComponent>()
    {
        return "Sprite Component";
    }
    
    template<>
    void DrawComponent<SpriteComponent>(Entity& e)
    {
        SpriteComponent& component = e.GetComponent<SpriteComponent>();
        ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
        ImGui::Button("Texture", { 100.f, 0.f });
    }

    template<>
    void SerializeComponent<SpriteComponent>(const Entity& e)
    {
        
    }
    
    template<>
    void DeserializeComponent<SpriteComponent>(Entity& e)
    {
        
    }
    // END TODO REFLECTOR
}