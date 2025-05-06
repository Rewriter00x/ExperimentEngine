#pragma once

namespace Exp
{
    class NativeScript;
}

namespace Exp
{
    NativeScript* CreateScriptByName(const std::string& name, Entity_ID entityID, World* world);

    const std::vector<std::string>& GetAllScriptNames();
    const std::vector<const char*>& GetAllScriptNamesPtrs();
}
