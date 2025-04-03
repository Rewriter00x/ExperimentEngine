#include "exppch.h"
#include "ModuleManager.h"

namespace Exp
{
    void ModuleManager::OnUpdate(float deltaSeconds) const
    {
        for (const Unique<Module>& module : m_Modules)
        {
            module->OnUpdate(deltaSeconds);
        }
    }

    void ModuleManager::OnImGuiRender() const
    {
        for (const Unique<Module>& module : m_Modules)
        {
            module->OnImGuiRender();
        }
    }
}
