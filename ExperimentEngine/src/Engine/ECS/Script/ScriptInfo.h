#pragma once

#include "ScriptUtils.h"

namespace Exp
{
    struct ScriptInfo
    {
        ScriptInfo() = default;
        ScriptInfo(const ScriptInfo& other)
            : Name(other.Name), Properties(other.Properties->Duplicate()) {}
        
        ScriptInfo& operator=(const ScriptInfo& other)
        {
            Name = other.Name;
            Properties = other.Properties->Duplicate();
            return *this;
        }
        
        std::string Name = "None";
        Unique<ScriptPropertiesBase> Properties = MakeUnique<ScriptPropertiesBase>();
    };
}
