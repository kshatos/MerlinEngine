#include "Merlin/Scene/game_scene.hpp"
#include "Merlin/Render/renderer.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/transform.hpp"
#include "Merlin/Scene/core_components.hpp"


namespace Merlin
{
    void GameScene::RenderScene()
    {
        Renderer::BeginScene(m_camera);
        for (const auto& entity : m_entities)
        {
            auto point_light_comp = entity->GetComponent<PointLightComponent>();
            if (point_light_comp != nullptr)
            {
                Renderer::AddLight(point_light_comp->data);
            }

            auto directional_light_comp = entity->GetComponent<DirectionalLightComponent>();
            if (directional_light_comp != nullptr)
            {
                Renderer::AddLight(directional_light_comp->data);
            }

            auto spot_light_component = entity->GetComponent<SpotLightComponent>();
            if (spot_light_component != nullptr)
            {
                Renderer::AddLight(spot_light_component->data);
            }
        }

        for (const auto& entity : m_entities)
        {
            auto mesh_comp = entity->GetComponent<MeshRenderComponent>();
            auto transform_comp = entity->GetComponent<TransformComponent>();
            if (mesh_comp != nullptr && transform_comp != nullptr)
            {
                Renderer::Submit(
                    mesh_comp->shader,
                    mesh_comp->varray,
                    transform_comp->transform.GetTransformationMatrix());
            }
        }

        Renderer::EndScene();
    }

    void GameScene::OnAwake()
    {
        for (const auto& entity : m_entities)
        {
            entity->OnAwake();
        }
    }

    void GameScene::OnUpdate(float time_step)
    {
        for (const auto& entity : m_entities)
        {
            entity->OnUpdate(time_step);
        }
    }

}