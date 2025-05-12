#include "exppch.h"
#include "DefaultConfig.h"

#include "yaml-cpp/yaml.h"

#include "imgui.h"
#include "Engine/ImGui/ExpImGui.h"

namespace Exp
{
    void DefaultConfig::Draw()
    {
        const std::string pathString = DefaultWorldPath.generic_string();
        std::string path = pathString;
        
        ImGui::InputText("Default World Path", path.data(), path.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
        
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ExpImGui::g_ImGuiContent_WorldFile))
            {
                const char* pathData = static_cast<const char*>(payload->Data);
                std::filesystem::path acceptedPath(pathData);
                path = std::filesystem::relative(acceptedPath, g_RootDirectory).generic_string();
            }
            ImGui::EndDragDropTarget();
        }
        
        if (ImGui::Button("Reset"))
        {
            path = "";
        }
        
        if (path != pathString)
        {
            DefaultWorldPath = path;
        }
    }

    void DefaultConfig::Serialize(YAML::Emitter& out) const
    {
        out << YAML::Key << "DefaultWorldPath" << YAML::Value << DefaultWorldPath.generic_string();
    }

    void DefaultConfig::Deserialize(const YAML::Node& node)
    {
        DefaultWorldPath = node["DefaultWorldPath"].as<std::string>();
    }
}
