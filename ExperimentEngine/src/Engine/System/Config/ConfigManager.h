#pragma once

#include "ConfigBase.h"

namespace Exp
{
    struct ConfigWrapper
    {
        Unique<ConfigBase> Config;
        std::filesystem::path Path;
    };

    class ConfigManager
    {
    public:
        template<typename T>
        void AddConfig(const std::filesystem::path& path)
        {
            m_TypeToConfig[typeid(T)] = { MakeUnique<T>(), path };
            AddConfig(path, typeid(T));
        }
        
        template<typename T>
        T& GetConfig() { return * (T*) m_TypeToConfig.at(typeid(T)).Config.get(); }
        
        template<typename T>
        const T& GetConfig() const { return * (T*) m_TypeToConfig.at(typeid(T)).Config.get(); }
        
        template<typename T>
        void SaveConfig() const { SaveConfig(typeid(T)); }
        
        template<typename T>
        void DrawConfig() { DrawConfig(typeid(T)); }
        
    private:
        void AddConfig(const std::filesystem::path& path, std::type_index id);
        
        void SaveConfig(std::type_index id) const;
        
        void DrawConfig(std::type_index id);
        
        std::unordered_map<std::type_index, ConfigWrapper> m_TypeToConfig;
        
    };
}
