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

                auto rotation =
                    transform_comp.transform.GetEulerAnglesDegrees();
                std::vector<float> rotation_buffer{
                    rotation.x, rotation.y, rotation.z};
                if (ImGui::InputFloat3("Rotation", rotation_buffer.data()))
                {
                    transform_comp.transform.SetEulerAnglesDegrees(
                        glm::vec3(rotation_buffer[0],
                                  rotation_buffer[1],
                                  rotation_buffer[2]));
                }

                auto scale = transform_comp.transform.GetScale();
                std::vector<float> scale_buffer{scale.x, scale.y, scale.z};
                if (ImGui::InputFloat3("Scale", scale_buffer.data()))
                {
                    transform_comp.transform.SetScale(glm::vec3(
                        scale_buffer[0], scale_buffer[1], scale_buffer[2]));
                }
                ImGui::TreePop();
            }

            if (m_inspected_entity->HasComponent<Merlin::PointLightComponent>())
            {
                auto& point_light =
                    m_inspected_entity
                        ->GetComponent<Merlin::PointLightComponent>();
                if (ImGui::TreeNodeEx("Point Light", flags))
                {
                    std::vector<float> color_buffer{
                        point_light.m_data.color.r,
                        point_light.m_data.color.g,
                        point_light.m_data.color.b,
                    };
                    if (ImGui::ColorEdit3("Color",
                                          color_buffer.data(),
                                          ImGuiColorEditFlags_DefaultOptions_))
                    {
                        point_light.m_data.color.x = color_buffer[0];
                        point_light.m_data.color.y = color_buffer[1];
                        point_light.m_data.color.z = color_buffer[2];
                    }
                    if (ImGui::InputFloat("Radiant Flux",
                                          &point_light.m_data.radiant_flux))
                    {
                        point_light.m_data.radiant_flux =
                            std::max(0.0f, point_light.m_data.radiant_flux);
                    }
                    if (ImGui::InputFloat("Range", &point_light.m_data.range))
                    {
                        point_light.m_data.range =
                            std::max(0.0f, point_light.m_data.range);
                    }

                    ImGui::TreePop();
                }
            }

            if (m_inspected_entity
                    ->HasComponent<Merlin::DirectionalLightComponent>())
            {
                auto& directional_light =
                    m_inspected_entity
                        ->GetComponent<Merlin::DirectionalLightComponent>();
                if (ImGui::TreeNodeEx("Directional Light", flags))
                {
                    std::vector<float> color_buffer{
                        directional_light.m_data.color.r,
                        directional_light.m_data.color.g,
                        directional_light.m_data.color.b,
                    };
                    if (ImGui::ColorEdit3("Color",
                                          color_buffer.data(),
                                          ImGuiColorEditFlags_DefaultOptions_))
                    {
                        directional_light.m_data.color.x = color_buffer[0];
                        directional_light.m_data.color.y = color_buffer[1];
                        directional_light.m_data.color.z = color_buffer[2];
                    }
                    if (ImGui::InputFloat("Irradiance",
                                          &directional_light.m_data.irradiance))
                    {
                        directional_light.m_data.irradiance =
                            std::max(0.0f, directional_light.m_data.irradiance);
                    }
                    ImGui::TreePop();
                }
            }

            if (m_inspected_entity->HasComponent<Merlin::SpotLightComponent>())
            {
                auto& spot_light =
                    m_inspected_entity
                        ->GetComponent<Merlin::SpotLightComponent>();
                if (ImGui::TreeNodeEx("Spot Light", flags))
                {
                    std::vector<float> color_buffer{
                        spot_light.m_data.color.r,
                        spot_light.m_data.color.g,
                        spot_light.m_data.color.b,
                    };
                    if (ImGui::ColorEdit3("Color",
                                          color_buffer.data(),
                                          ImGuiColorEditFlags_DefaultOptions_))
                    {
                        spot_light.m_data.color.x = color_buffer[0];
                        spot_light.m_data.color.y = color_buffer[1];
                        spot_light.m_data.color.z = color_buffer[2];
                    }
                    if (ImGui::InputFloat("Radiant Intensity",
                                          &spot_light.m_data.radiant_intensity))
                    {
                        spot_light.m_data.radiant_intensity =
                            std::max(0.0f, spot_light.m_data.radiant_intensity);
                    }
                    if (ImGui::InputFloat("Range", &spot_light.m_data.range))
                    {
                        spot_light.m_data.range =
                            std::max(0.0f, spot_light.m_data.range);
                    }
                    if (ImGui::InputFloat("Cutoff Angle",
                                          &spot_light.m_data.cutoff_angle))
                    {
                        spot_light.m_data.cutoff_angle =
                            std::max(0.0f, spot_light.m_data.cutoff_angle);
                    }

                    ImGui::TreePop();
                }
            }

            if (m_inspected_entity->HasComponent<Merlin::MeshRenderComponent>())
            {
                auto& mesh_render_component =
                    m_inspected_entity
                        ->GetComponent<Merlin::MeshRenderComponent>();

                if (ImGui::TreeNodeEx("Mesh Render", flags))
                {
                    // TODO:
                    // Show mesh asset used (need UUID & asset registry?)
                    // Drag & drop from asset viewer
                    // Popup to search through registered meshes
                    ImGui::TreePop();
                }
            }

            if (m_inspected_entity->HasComponent<Merlin::CameraComponent>())
            {
                auto& camera_component =
                    m_inspected_entity->GetComponent<Merlin::CameraComponent>();

                if (ImGui::TreeNodeEx("Camera", flags))
                {
                    // TODO:
                    // Dropdown for camera type selection
                    // Skybox asset references
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}  // namespace MerlinEditor