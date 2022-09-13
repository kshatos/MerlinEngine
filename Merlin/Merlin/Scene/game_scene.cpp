#include "Merlin/Scene/game_scene.hpp"

#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/transform.hpp"
#include "Merlin/Scene/core_components.hpp"
#include "Merlin/Scene/entity.hpp"

namespace Merlin
{
    GameScene::GameScene() {}

    GameScene::~GameScene() {}

    Entity GameScene::CreateEntity(UUID uuid)
    {
        auto entity_handle = m_registry.create();
        Entity entity(entity_handle, this);
        auto info_component = entity.AddComponent<EntityInfoComponent>(uuid);
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<EntityTreeComponent>();

        m_uuid_entity_map[info_component.uuid] = entity_handle;

        return entity;
    }

    std::optional<Entity> GameScene::GetEntity(UUID uuid)
    {
        std::optional<Entity> entity;
        auto map_iterator = m_uuid_entity_map.find(uuid);
        if (map_iterator != m_uuid_entity_map.end())
        {
            auto handle = map_iterator->second;
            entity = Entity(handle, this);
        }
        return entity;
    }

    const SceneRenderData& GameScene::GetRenderData() { return m_render_data; }

    void GameScene::OnAwake() {}

    void GameScene::OnUpdate(float time_step)
    {
        m_render_data.meshes.clear();
        m_render_data.directional_lights.clear();
        m_render_data.point_lights.clear();
        m_render_data.spot_lights.clear();

        {
            auto view = m_registry.view<MeshRenderComponent>();
            view.each(
                [this](entt::entity entity, MeshRenderComponent& component)
                {
                    auto transform = m_registry.get<TransformComponent>(entity);
                    component.m_data.model_matrix =
                        transform.transform.GetTransformationMatrix();
                    m_render_data.meshes.push_back(&component.m_data);
                });
        }
        {
            auto view = m_registry.view<SpotLightComponent>();
            view.each(
                [this](entt::entity entity, SpotLightComponent& component)
                {
                    auto transform = m_registry.get<TransformComponent>(entity);
                    component.m_data.position =
                        transform.transform.GetPosition();
                    component.m_data.direction = transform.transform.Forward();
                    m_render_data.spot_lights.push_back(&component.m_data);
                });
        }
        {
            auto view = m_registry.view<PointLightComponent>();
            view.each(
                [this](entt::entity entity, PointLightComponent& component)
                {
                    auto transform = m_registry.get<TransformComponent>(entity);
                    component.m_data.position =
                        transform.transform.GetPosition();
                    m_render_data.point_lights.push_back(&component.m_data);
                });
        }
        {
            auto view = m_registry.view<DirectionalLightComponent>();
            view.each(
                [this](entt::entity entity,
                       DirectionalLightComponent& component)
                {
                    auto transform = m_registry.get<TransformComponent>(entity);
                    m_render_data.directional_lights.push_back(
                        &component.m_data);
                });
        }
        {
            auto view = m_registry.view<CameraComponent>();
            view.each(
                [this](entt::entity entity, CameraComponent& camera_component)
                {
                    auto& transform_component =
                        m_registry.get<TransformComponent>(entity);

                    CameraRenderData camera_data;
                    camera_data.projection_matrix =
                        camera_component.projection->GetProjectionMatrix();
                    camera_data.view_matrix =
                        glm::inverse(transform_component.transform
                                         .GetTransformationMatrix());
                    camera_data.view_pos =
                        transform_component.transform.GetPosition();

                    m_render_data.camera = camera_data;
                });
        }
    }

    void GameScene::VisitEntities(std::function<void(Entity)> callback)
    {
        m_registry.each([callback, this](entt::entity entity)
                        { callback(Entity(entity, this)); });
    }

}  // namespace Merlin