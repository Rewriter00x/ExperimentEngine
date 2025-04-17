#pragma once

#include "ExperimentEngine.h"

namespace Exp
{
    struct ContentBrowserItem
    {
        std::string Name;
        bool IsDirectory = false;

        ContentBrowserItem() = default;
        ContentBrowserItem(const std::string& name, bool isDirectory)
            : Name(name), IsDirectory(isDirectory) {}
    };
    
    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel(const std::string& name, const std::filesystem::path& rootPath);

        void OnImGuiRender();

    private:
        void CacheCurrentDir();

        const std::string m_Name;
        
        const std::filesystem::path m_RootPath;
        std::filesystem::path m_CurrentDir;

        Shared<Texture> m_DirectoryIcon;
        Shared<Texture> m_FileIcon;

        std::vector<ContentBrowserItem> m_CachedItems;
        
    };
}
