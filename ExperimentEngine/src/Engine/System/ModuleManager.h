#pragma once

namespace Exp
{
    class ModuleManager
    {
    public:
        template<typename T, typename ... Args>
        void AddModule(Args&& ... args);

        void OnUpdate(float deltaSeconds) const;
        void OnImGuiRender() const;

        const std::vector<Unique<Module>>& GetModules() const { return m_Modules; }

    private:
        std::vector<Unique<Module>> m_Modules;
        
    };
}

namespace Exp
{
    template <typename T, typename ... Args>
    void ModuleManager::AddModule(Args&& ... args)
    {
        m_Modules.push_back(MakeUnique<T>(std::forward<Args>(args)...));
    }
}
