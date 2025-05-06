#include "exppch.h"
#include "Serializer.h"

#include "Engine/Render/RenderData/Texture.h"

#include "yaml-cpp/yaml.h"

#include "ExpYaml.h"

namespace Exp::Serializer
{
    static void SerializeEntity(YAML::Emitter& out, const Entity& entity)
    {
        out << YAML::BeginMap;
        
        out << YAML::Key << "UUID" << YAML::Value << entity.GetUUID();
        out << YAML::Key << "Name" << YAML::Value << entity.GetName();

        out << YAML::Key << "Position" << YAML::Value << entity.GetPosition();
        out << YAML::Key << "Rotation" << YAML::Value << entity.GetRotation();
        out << YAML::Key << "Scale" << YAML::Value << entity.GetScale();

        for (const ComponentWrapperBase* component : GetAllComponents())
        {
            if (component->ContainedBy(entity))
            {
                component->Serialize(out, entity);
            }
        }
        
        out << YAML::EndMap;
    }
    
    void Serialize(const Shared<World>& world, const std::filesystem::path& path)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "World" << YAML::Value << world->GetName();
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        for (const Entity& entity : world->GetEntities())
        {
            SerializeEntity(out, entity);
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(path);
        EXP_ASSERT(fout);
        fout << out.c_str();
    }

    bool Deserialize(Shared<World>& world, const std::filesystem::path& path)
    {
        YAML::Node data;
        try
        {
            data = YAML::LoadFile(path.string());
        }
        catch (const YAML::ParserException& e)
        {
            return false;
        }

        if (!data["World"])
        {
            return false;
        }

        const std::string worldName = data["World"].as<std::string>();

        if (const YAML::Node entities = data["Entities"])
        {
            for (auto entityData : entities)
            {
                const UUID uuid = entityData["UUID"].as<uint64>();
                const std::string name = entityData["Name"].as<std::string>();

                const glm::vec3 position = entityData["Position"].as<glm::vec3>();
                const glm::vec3 rotation = entityData["Rotation"].as<glm::vec3>();
                const glm::vec3 scale = entityData["Scale"].as<glm::vec3>();

                Entity& entity = world->CreateEntity(name, uuid);
                entity.SetPosition(position);
                entity.SetRotation(rotation);
                entity.SetScale(scale);
                
                for (const ComponentWrapperBase* component : GetAllComponents())
                {
                    if (const YAML::Node componentData = entityData[component->GetName()])
                    {
                        component->Deserialize(componentData, entity);
                    }
                }
            }
        }
        
        return true;
    }
    
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const Shared<Texture>& t)
    {
        out << (t ? std::filesystem::relative(t->GetFilepath(), g_RootDirectory).generic_string() : "");
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const ScriptName& s)
    {
        out << s.Name;
        return out;
    }
}
