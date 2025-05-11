#include "exppch.h"
#include "DefaultConfig.h"

#include "yaml-cpp/yaml.h"

#include "imgui.h"
#include "Engine/ImGui/ExpImGui.h"

namespace Exp
{
    void DefaultConfig::Draw()
    {
        const std::string pathString = std::filesystem::relative(DefaultWorldPath, g_RootDirectory).generic_string();
        std::string path = pathString;
        ImGui::InputText("Default World Path", path.data(), path.capacity() + 1, ImGuiInputTextFlags_CallbackResize, ExpImGui::InputTextCallback, &path);
        if (path != pathString)
        {
            DefaultWorldPath = g_RootDirectory / path;
        }
    }

    void DefaultConfig::Serialize(YAML::Emitter& out) const
    {
        out << YAML::Key << "DefaultWorldPath" << YAML::Value << std::filesystem::relative(DefaultWorldPath, g_RootDirectory).generic_string();
    }

    void DefaultConfig::Deserialize(const YAML::Node& node)
    {
        DefaultWorldPath = g_RootDirectory / node["DefaultWorldPath"].as<std::string>();
    }
}
