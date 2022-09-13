#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP
#include <entt/entt.hpp>
#include <memory>

#include "Merlin/Render/camera_projection.hpp"
#include "Merlin/Render/scene_render_data.hpp"
#include "Merlin/Render/skybox.hpp"
#include "Merlin/Scene/core_components.hpp"

namespace Merlin
{
    class Entity;

    class GameScene
    {
        entt::registry m_registry;
        SceneRenderData m_render_data;
        std::unordered_map<UUID, entt::entity> m_uuid_entity_map;

    public:
        GameScene();
        GameScene(const GameScene& other) = delete;
        GameScene(GameScene&& other) = delete;
        GameScene& operator=(const GameScene& rhs) = delete;
        GameScene& operator=(GameScene&& rhs) = delete;
        ~GameScene();

        Entity CreateEntity(UUID uuid = UUID());
        std::optional<Entity> GetEntity(UUID uuid);
        void GameScene::OnAwake();
        void OnUpdate(float timestep);
        void VisitEntities(std::function<void(Entity)> callback);
        template <typename... Args>
        void VisitEntities(std::function<void(Entity, Args&...)> callback)
        {
            auto view = m_registry.view<Args...>();
            view.each([callback, this](entt::entity entity, Args&... args)
                      { callback(Entity(entity, this), args...); });
        }
        const SceneRenderData& GetRenderData();
        inline void SetAmbientLight(float radiance)
        {
            m_render_data.ambient_light_radiance = radiance;
        }

    private:
        friend class Entity;
        friend class SceneSerializer;
    };
}  // namespace Merlin

#endif