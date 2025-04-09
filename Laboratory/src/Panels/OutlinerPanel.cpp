#include "OutlinerPanel.h"

#include "imgui.h"

namespace Exp
{
    static int InputTextCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            std::string* name = (std::string*)data->UserData;
            const int32 cap = (int32)name->capacity();
            if (cap <= data->BufTextLen)
            {
                name->resize((size_t)data->BufTextLen * 2);
                EXP_LOG(Info, "Resize from %d to %d", cap, data->BufTextLen * 2);
            }
            else if (cap > data->BufTextLen * 2)
            {
                name->resize((size_t)data->BufTextLen);
                name->shrink_to_fit();
                EXP_LOG(Info, "Resize from %d to %d", cap, data->BufTextLen);
            }
            data->Buf = name->data();
        }
        return 0;
    }
    
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

        std::vector<Entity>& entities = m_World->GetEntities();
        
        for (int32 i = 0; i < (int32)entities.size(); i++)
        {
            const Entity& entity = entities[i];
            const std::string label = entity.GetName() + "##" + std::to_string(i);
            const bool selected = m_SelectedEntityID == i;
            if (ImGui::Selectable(label.c_str(), selected))
            {
                SetSelectedEntityID(i);
            }
        }

        ImGui::End();

        ImGui::Begin("Details");

        if (m_SelectedEntityID != -1)
        {
            Entity& selectedEntity = entities[m_SelectedEntityID];
            
            ImGui::DragFloat3("Position", glm::value_ptr(selectedEntity.GetPosition()), .1f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(selectedEntity.GetRotation()));
            ImGui::DragFloat3("Scale", glm::value_ptr(selectedEntity.GetScale()), .1f);

            std::string& name = selectedEntity.GetName();
            ImGui::InputText("Name", name.data(), name.capacity() + 1, ImGuiInputTextFlags_CallbackResize, InputTextCallback, &name);

            ImGui::Text("ID: %d", (int32)selectedEntity.GetID());
            ImGui::Text("UUID: %d", (int32)selectedEntity.GetUUID());
        }

        ImGui::End();
    }

    void OutlinerPanel::SetWorld(const Shared<World>& world)
    {
        m_World = world;
        m_SelectedEntityID = -1;
    }
    
    void OutlinerPanel::SetSelectedEntityID(int32 id)
    {
        m_SelectedEntityID = id;
    }
}
