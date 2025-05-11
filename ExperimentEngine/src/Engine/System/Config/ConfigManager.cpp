#include "exppch.h"
#include "ConfigManager.h"

#include "yaml-cpp/yaml.h"

namespace Exp
{
    void ConfigManager::AddConfig(const std::filesystem::path& path, std::type_index id)
    {
        const std::filesystem::path fullPath = g_RootDirectory / path;
        if (std::filesystem::exists(fullPath))
        {
            YAML::Node data;
            try
            {
                data = YAML::LoadFile(fullPath.string());
            }
            catch (const YAML::ParserException& e)
            {
                return;
            }
            
            m_TypeToConfig[id].Config->Deserialize(data);
        }
    }

    void ConfigManager::SaveConfig(std::type_index id) const
    {
        const ConfigWrapper& config = m_TypeToConfig.at(id);
        
        YAML::Emitter out;
        out << YAML::BeginMap;
        config.Config->Serialize(out);
        out << YAML::EndMap;
        
        std::ofstream fout(g_RootDirectory / config.Path);
        EXP_ASSERT(fout);
        fout << out.c_str();
    }

    void ConfigManager::DrawConfig(std::type_index id)
    {
        ConfigWrapper& config = m_TypeToConfig.at(id);
        config.Config->Draw();
    }
}
