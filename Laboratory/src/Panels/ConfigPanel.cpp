#include "ConfigPanel.h"

#include "Engine/System/Config/DefaultConfig.h"

#include "imgui.h"

namespace Exp
{
    void ConfigPanel::OnImGuirender()
    {
        ImGui::Begin("Default Config");
        Application::Get().GetConfigManager().DrawConfig<DefaultConfig>();
        if (ImGui::Button("Save"))
        {
            Application::Get().GetConfigManager().SaveConfig<DefaultConfig>();
        }
        ImGui::End();
    }
}
