#pragma once

namespace YAML
{
    class Emitter;
}

namespace Exp
{
    struct ScriptName;
}

namespace Exp
{
    namespace Serializer
    {
        void Serialize(const Shared<World>& world, const std::filesystem::path& path);
        bool Deserialize(Shared<World>& world, const std::filesystem::path& path);

        YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v);
        YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);
        YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);
        YAML::Emitter& operator<<(YAML::Emitter& out, const Shared<Texture>& t);
        YAML::Emitter& operator<<(YAML::Emitter& out, const ScriptName& s);
    }
}
