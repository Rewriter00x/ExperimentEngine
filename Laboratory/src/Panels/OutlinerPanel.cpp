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
            }
            else if (cap > data->BufTextLen * 2)
            {
                name->resize((size_t)data->BufTextLen);
                name->shrink_to_fit();
            }
            data->Buf = name->data();
        }
        return 0;
    }

    static void DrawEntityProperties(Entity& e)
    {
        ImGui::DragFloat3("Position", glm::value_ptr(e.GetPosition()), .1f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(e.GetRotation()));
        ImGui::DragFloat3("Scale", glm::value_ptr(e.GetScale()), .1f);

        std::string& name = e.GetName();
        ImGui::InputText("Name", name.data(), name.capacity() + 1, ImGuiInputTextFlags_CallbackResize, InputTextCallback, &name);

        ImGui::Text("ID: %d", (int32)e.GetID());
        ImGui::Text("UUID: %llu", (uint64)e.GetUUID());
    }
    
    OutlinerPanel::OutlinerPanel(const Shared<World>& world)
    {
        SetWorld(world);
    }

    void OutlinerPanel::OnImGuiRender()
    {
        ImGui::Begin("Outliner");

        if (ImGui::BeginPopupContextWindow("OutlinerPopup", 1 | ImGuiPopupFlags_NoOpenOverItems))
        {
            if (ImGui::MenuItem("Add Entity"))
            {
                m_World->CreateEntity("New Entity");
            }
            ImGui::EndPopup();
        }
        
        const std::vector<Entity> entitiesCopy = m_World->GetEntities();
        for (int32 i = 0; i < (int32)entitiesCopy.size(); i++)
        {
            const Entity& entity = entitiesCopy[i];
            const std::string label = entity.GetName() + "##" + std::to_string(i);
            const bool selected = m_SelectedEntityID == i;
            if (ImGui::Selectable(label.c_str(), selected))
            {
                SetSelectedEntityID(i);
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Remove Entity"))
                {
                    if (selected)
                    {
                        ClearSelectedEntity();
                    }
                    else if (m_SelectedEntityID > i)
                    {
                        m_SelectedEntityID--;
                        ClearSelectedComponent();
                    }
                    m_World->DestroyEntity(entity);
                }
                
                ImGui::EndPopup();
            }
        }

        ImGui::End();

        ImGui::Begin("Details");

        std::vector<Entity>& entities = m_World->GetEntities();
        if (m_SelectedEntityID != -1)
        {
            Entity& selectedEntity = entities[m_SelectedEntityID];
            
            ImGui::Separator();
            DrawComponentsList(selectedEntity, AllComponents{});
            ImGui::Separator();

            if (typeid(*m_SelectedComponent) == typeid(SelectedComponentBase))
            {
                DrawEntityProperties(selectedEntity);
            }
            else
            {
                m_SelectedComponent->Draw(selectedEntity);
            }
        }

        ImGui::End();
    }

    void OutlinerPanel::SetWorld(const Shared<World>& world)
    {
        m_World = world;
        ClearSelectedEntity();
    }
    
    void OutlinerPanel::SetSelectedEntityID(int32 id)
    {
        m_SelectedEntityID = id;
    }

    void OutlinerPanel::ClearSelectedEntity()
    {
        m_SelectedEntityID = -1;
        ClearSelectedComponent();
    }

    void OutlinerPanel::ClearSelectedComponent()
    {
        m_SelectedComponent = MakeUnique<SelectedComponentBase>();
    }

    template<typename... Components>
    void OutlinerPanel::DrawComponentsList(Entity& e, ComponentList<Components...>)
    {
        if (ImGui::Button("Add Components"))
        {
            ImGui::OpenPopup("AddComponents");
        }

        if (ImGui::BeginPopup("AddComponents"))
        {
            ([&e]
            {
                const char* name = GetComponentName<Components>();
                if (!e.HasComponent<Components>())
                {
                    if (ImGui::MenuItem(name))
                    {
                        e.AddComponent<Components>();
                        ImGui::CloseCurrentPopup();
                    }
                }
            }(), ...);

            ImGui::EndPopup();
        }
        
        ([this, &e]
        {
            const char* name = GetComponentName<Components>();
            if (e.HasComponent<Components>())
            {
                const bool selected = typeid(SelectedComponent<Components>) == typeid(*m_SelectedComponent);
                if (ImGui::Selectable(name, selected))
                {
                    m_SelectedComponent = selected ? MakeUnique<SelectedComponentBase>() : MakeUnique<SelectedComponent<Components>>();
                }
                if (ImGui::BeginPopupContextItem(name))
                {
                    if (ImGui::MenuItem("Remove Component"))
                    {
                        if (typeid(SelectedComponent<Components>) == typeid(*m_SelectedComponent))
                        {
                            ClearSelectedComponent();
                        }
                        e.RemoveComponent<Components>();
                    }
                    ImGui::EndPopup();
                }
            }
        }(), ...);
    }
}
