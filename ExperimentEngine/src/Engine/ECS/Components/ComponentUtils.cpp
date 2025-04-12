#include "exppch.h"
#include "ComponentUtils.h"

#include "imgui.h"
#include "yaml-cpp/yaml.h"
#include "Engine/Serialization/Serializer.h"

#include "SpriteComponent.h"

namespace Exp
{
    // BEGIN TODO REFLECTOR
    static ComponentWrapper<SpriteComponent> spriteComponent;
    
    static std::vector<ComponentWrapperBase*> s_AllComponents = { &spriteComponent };
    
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
    void SerializeComponent<SpriteComponent>(YAML::Emitter& out, const Entity& e)
    {
        using namespace Serializer;
        
        const SpriteComponent& component = e.GetComponent<SpriteComponent>();
        out << YAML::Key << GetComponentName<SpriteComponent>();
        out << YAML::BeginMap;
        out << YAML::Key << "Color" << YAML::Value << component.Color;
        //out << YAML::Key << "Texture" << YAML::Value << component.SpriteTexture;
        out << YAML::EndMap;
    }
    
    template<>
    void DeserializeComponent<SpriteComponent>(const YAML::Node& node, Entity& e)
    {
        SpriteComponent& component = e.AddComponent<SpriteComponent>();
        component.Color = node["Color"].as<glm::vec4>();
    }
    // END TODO REFLECTOR

    const std::vector<ComponentWrapperBase*>& GetAllComponents()
    {
        return s_AllComponents;
    }
}
