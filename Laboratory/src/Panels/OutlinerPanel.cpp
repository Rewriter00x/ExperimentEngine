#include "OutlinerPanel.h"

#include "imgui.h"

namespace Exp
{
    OutlinerPanel::OutlinerPanel(const Shared<World>& world)
    {
        SetWorld(world);
    }

    void OutlinerPanel::OnUpdate(float deltaSeconds)
    {
        const std::vector<Entity>& entities = m_World->GetEntities();
        if (m_SelectedEntityID >= (int32)entities.size())
        {
            m_SelectedEntityID = -1;
        }
    }

    void OutlinerPanel::OnImGuiRender()
    {
        ImGui::Begin("Outliner");

        const std::vector<Entity>& entities = m_World->GetEntities();
        for (int32 i = 0; i < (int32)entities.size(); i++)
        {
            const Entity& entity = entities[i];
            const std::string label = entity.GetName() + "##" + std::to_string(i);
            const bool selected = m_SelectedEntityID != -1 && entities[m_SelectedEntityID] == entity;
            if (ImGui::Selectable(label.c_str(), selected))
            {
                SetSelectedEntity(i);
            }
        }

        ImGui::End();

        ImGui::Begin("Details");

        ImGui::End();
    }

    void OutlinerPanel::SetWorld(const Shared<World>& world)
    {
        m_World = world;
        m_SelectedEntityID = -1;
    }
    
    void OutlinerPanel::SetSelectedEntity(int32 id)
    {
        m_SelectedEntityID = id;
    }
}
