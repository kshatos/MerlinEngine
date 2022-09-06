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
        if (m_scene == nullptr)
        {
            ImGui::End();
            return;
        }
        m_scene->VisitEntities([this](Merlin::Entity entity)
                               { DrawEntity(entity); });
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntity(Merlin::Entity entity)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        if (m_selected_entity.has_value() &&
            m_selected_entity.value() == entity)
            flags |= ImGuiTreeNodeFlags_Selected;

        if (ImGui::TreeNodeEx("Entity", flags))
        {
            ImGui::TreePop();
        }
        if (ImGui::IsItemClicked())
        {
            m_selected_entity = entity;
        }
    }
}  // namespace MerlinEditor