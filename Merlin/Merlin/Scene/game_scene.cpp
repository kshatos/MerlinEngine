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
        Renderer::RenderScene(m_render_data);
    }

    std::shared_ptr<Entity> GameScene::CreateEntity()
    {
        auto entity = std::shared_ptr<Entity>(new Entity());
        entity->component_added_callback = 
            [this](std::shared_ptr<Component> component){OnComponentAdded(component); };
        m_entities.push_back(entity);
        return entity;
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

    void GameScene::OnComponentAdded(std::shared_ptr<Component> component)
    {
        auto camera_component = std::dynamic_pointer_cast<CameraComponent>(component);
        if (camera_component)
        {
            m_render_data.camera = &camera_component->data;
        }

        auto point_light_comp = std::dynamic_pointer_cast<PointLightComponent>(component);
        if (point_light_comp != nullptr)
        {
            m_render_data.point_lights.push_back(&point_light_comp->data);
        }

        auto directional_light_comp = std::dynamic_pointer_cast<DirectionalLightComponent>(component);
        if (directional_light_comp != nullptr)
        {
            m_render_data.directional_lights.push_back(&directional_light_comp->data);
        }

        auto spot_light_component = std::dynamic_pointer_cast<SpotLightComponent>(component);
        if (spot_light_component != nullptr)
        {
            m_render_data.spot_lights.push_back(&spot_light_component->data);
        }

        auto mesh_render_comp = std::dynamic_pointer_cast<MeshRenderComponent>(component);
        if (mesh_render_comp != nullptr)
        {
            m_render_data.meshes.push_back(&mesh_render_comp->data);
        }
    }

}