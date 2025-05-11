#pragma once

namespace YAML
{
    class Emitter;
    class Node;
}

namespace Exp
{
    struct ConfigBase
    {
        virtual void Draw() {}
        virtual void Serialize(YAML::Emitter& out) const {}
        virtual void Deserialize(const YAML::Node& node) {}
    };
}
