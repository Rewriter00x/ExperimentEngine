#pragma once

namespace Exp
{
    // wrapper for reflection system
    struct ScriptName
    {
        std::string Name = "None";

        operator const std::string&() const { return Name; }
    };
}