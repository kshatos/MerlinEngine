#include "scene_hierarchy_panel.hpp"

#include <imgui.h>

namespace MerlinEditor
{

    SceneHierarchyPanel::SceneHierarchyPanel()
        : m_name("Scene Hierarchy"), m_scene(nullptr)
    {
    }

    void SceneHierarchyPanel::DrawPanel()
    {
        ImGui::Begin(m_name.c_str());

        if (m_scene != nullptr)
        {
            m_scene->VisitEntities([this](Merlin::Entity entity)
                                   { DrawEntity(entity); });

            if (ImGui::BeginPopupContextWindow(
                    "",
                    ImGuiPopupFlags_NoOpenOverItems |
                        ImGuiPopupFlags_MouseButtonRight))
            {
                if (ImGui::MenuItem("Create Entity"))
                {
                    m_scene->CreateEntity();
                }

                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntity(Merlin::Entity entity)
    {
        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
        if (m_selected_entity.has_value() &&
            m_selected_entity.value() == entity)
            flags |= ImGuiTreeNodeFlags_Selected;

        if (ImGui::TreeNodeEx((void*)entity.GetID(), flags, "Entity"))
        {
            ImGui::TreePop();
        }
        if (ImGui::IsItemClicked())
        {
            m_selected_entity = entity;
        }
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity")) entity.Destroy();

            ImGui::EndPopup();
        }
    }
}  // namespace MerlinEditor