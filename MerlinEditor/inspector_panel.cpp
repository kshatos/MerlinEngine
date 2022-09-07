#include "inspector_panel.hpp"

#include <imgui.h>

#include <Merlin/Scene/core_components.hpp>
#include <Merlin/Scene/game_scene.hpp>

namespace MerlinEditor
{
    void InspectorPanel::DrawPanel()
    {
        auto flags = ImGuiTreeNodeFlags_DefaultOpen |
                     ImGuiTreeNodeFlags_SpanAvailWidth |
                     ImGuiTreeNodeFlags_OpenOnArrow;

        ImGui::Begin(m_name.c_str());
        if (m_inspected_entity.has_value())
        {
            auto& entity_info =
                m_inspected_entity->GetComponent<Merlin::EntityInfoComponent>();
            if (ImGui::TreeNodeEx("Entity", flags))
            {
                std::vector<char> name_buffer(64);
                std::strcpy(name_buffer.data(), entity_info.name.c_str());
                if (ImGui::InputText(
                        "Name", name_buffer.data(), name_buffer.size()))
                {
                    entity_info.name = name_buffer.data();
                }
                ImGui::TreePop();
            }

            auto& transform_comp =
                m_inspected_entity->GetComponent<Merlin::TransformComponent>();
            if (ImGui::TreeNodeEx("Transform", flags))
            {
                auto position = transform_comp.transform.GetPosition();
                std::vector<float> position_buffer{
                    position.x, position.y, position.z};
                if (ImGui::InputFloat3("Position", position_buffer.data()))
                {
                    transform_comp.transform.SetPosition(
                        glm::vec3(position_buffer[0],
                                  position_buffer[1],
                                  position_buffer[2]));
                }

                auto scale = transform_comp.transform.GetScale();
                std::vector<float> rotation_buffer{scale.x, scale.y, scale.z};
                if (ImGui::InputFloat3("Scale", rotation_buffer.data()))
                {
                    transform_comp.transform.SetScale(
                        glm::vec3(rotation_buffer[0],
                                  rotation_buffer[1],
                                  rotation_buffer[2]));
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}  // namespace MerlinEditor