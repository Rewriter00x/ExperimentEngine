#pragma once

#include "ConfigBase.h"

namespace Exp
{
    struct DefaultConfig : public ConfigBase
    {
        std::filesystem::path DefaultWorldPath;
        
        virtual void Draw() override;
        virtual void Serialize(YAML::Emitter& out) const override;
        virtual void Deserialize(const YAML::Node& node) override;
    };
}
