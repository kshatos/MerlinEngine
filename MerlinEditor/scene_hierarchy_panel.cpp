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
                                   { DrawEntity(entity, true); });

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

        while (!m_deleted_entities.empty())
        {
            auto entity = m_deleted_entities.back();
            m_deleted_entities.pop_back();
            entity.Destroy();
        }
    }

    void SceneHierarchyPanel::DrawEntity(Merlin::Entity entity,
                                         bool first_layer)
    {
        auto& entity_info = entity.GetComponent<Merlin::EntityInfoComponent>();
        auto& entity_tree = entity.GetComponent<Merlin::EntityTreeComponent>();

        if (first_layer && entity_tree.parent.has_value())
            return;  // Only draw children on recursion

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        if (entity_tree.children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
        if (m_selected_entity.has_value() &&
            m_selected_entity.value() == entity)
            flags |= ImGuiTreeNodeFlags_Selected;

        auto open = ImGui::TreeNodeEx(
            (void*)entity.GetID(), flags, entity_info.name.c_str());
        if (ImGui::IsItemClicked())
        {
            m_selected_entity = entity;
        }
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                m_deleted_entities.push_back(entity);

            ImGui::EndPopup();
        }
        if (open)
        {
            for (auto& child : entity_tree.children)
            {
                DrawEntity(child, false);
            }
            ImGui::TreePop();
        }
    }
}  // namespace MerlinEditor