#pragma once

#include "yaml-cpp/yaml.h"

#include "Engine/Render/RenderData/Texture.h"
#include "Engine/ECS/Script/ScriptInfo.h"
#include "Engine/ECS/Script/ScriptUtils.h"

namespace YAML
{
    template<>
    struct convert<glm::vec2>
    {
        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3>
    {
        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Exp::Shared<Exp::Texture>>
    {
        static bool decode(const Node& node, Exp::Shared<Exp::Texture>& rhs)
        {
            if (node.IsSequence())
                return false;

            const std::string path = node.as<std::string>();
            if (!path.empty())
            {
                rhs = Exp::AssetManager::GetTexture(path);
            }
            return true;
        }
    };
    
    template<>
    struct convert<Exp::ScriptInfo>
    {
        static bool decode(const Node& node, Exp::ScriptInfo& rhs)
        {
            if (!node.IsMap())
                return false;

            rhs.Name = node["Name"].as<std::string>();
            rhs.Properties = Exp::CreateScriptPropertiesByName(rhs.Name);
            rhs.Properties->Deserialize(node["Properties"]);
            return true;
        }
    };
}
