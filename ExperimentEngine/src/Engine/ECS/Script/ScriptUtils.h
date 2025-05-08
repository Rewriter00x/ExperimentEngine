#pragma once

namespace YAML
{
    class Emitter;
    class Node;
}

namespace Exp
{
    class NativeScript;
}

namespace Exp
{
    struct ScriptPropertiesBase
    {
        virtual void Draw() {}
        virtual void Serialize(YAML::Emitter& out) const {}
        virtual void Deserialize(const YAML::Node& node) {}
        
        virtual Unique<ScriptPropertiesBase> Duplicate() const { return MakeUnique<ScriptPropertiesBase>(); }
        
        virtual void InitScript(NativeScript* script) const {}
    };

    NativeScript* CreateScriptByName(const std::string& name, Entity_ID entityID, World* world);
    Unique<ScriptPropertiesBase> CreateScriptPropertiesByName(const std::string& name);

    const std::vector<std::string>& GetAllScriptNames();
    const std::vector<const char*>& GetAllScriptNamesLiterals();
}
