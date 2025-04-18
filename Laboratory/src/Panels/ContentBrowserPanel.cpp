#include "ContentBrowserPanel.h"

#include "imgui.h"
#include "Engine/ImGui/ExpImGui.h"
#include "Engine/Render/RenderData/Texture.h"

namespace Exp
{
    ContentBrowserPanel::ContentBrowserPanel(const std::string& name, const std::filesystem::path& rootPath)
        : m_Name(name) 
        , m_RootPath(rootPath), m_CurrentDir(rootPath)
        , m_DirectoryIcon(AssetManager::GetTexture(g_EditorResourcesDirectory / "Textures" / "folder.png"))
        , m_FileIcon(AssetManager::GetTexture(g_EditorResourcesDirectory / "Textures" / "file.png"))
    {
        CacheCurrentDir();
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin(m_Name.c_str());

        static float padding = 16.f;
        static float thumbnailSize = 170.f * ExpImGui::GetOverallContentScale();

        const float cellSize = thumbnailSize + padding;

        if (m_CurrentDir != m_RootPath)
        {
            if (ImGui::Button("<-"))
            {
                m_CurrentDir = m_CurrentDir.parent_path();
                CacheCurrentDir();
            }
        }
        
        const float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;
        ImGui::Columns(columnCount, nullptr, false);

        bool needCache = false;
        for (const ContentBrowserItem& item : m_CachedItems)
        {
            const ImTextureID iconID = item.IsDirectory ? m_DirectoryIcon->GetRendererID() : m_FileIcon->GetRendererID();
            ImGui::ImageButton(item.Name.c_str(), iconID, { thumbnailSize, thumbnailSize }, { 0.f, 1.f }, { 1.f, 0.f });

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (item.IsDirectory)
                {
                    m_CurrentDir /= item.Name;
                    needCache = true;
                }
            }
            ImGui::Text("%s", item.Name.c_str());

            ImGui::NextColumn();
        }

        ImGui::Columns(1);

        ImGui::End();

        if (needCache)
        {
            CacheCurrentDir();
        }
    }

    void ContentBrowserPanel::CacheCurrentDir()
    {
        m_CachedItems.clear();
        for (auto& p : std::filesystem::directory_iterator(m_CurrentDir))
        {
            m_CachedItems.emplace_back(p.path().filename().string(), p.is_directory());
        }
    }
}
