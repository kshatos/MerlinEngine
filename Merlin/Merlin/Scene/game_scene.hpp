#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP
#include <entt/entt.hpp>
#include <memory>

#include "Merlin/Render/camera.hpp"
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

    public:
        GameScene();
        GameScene(const GameScene& other) = delete;
        GameScene(GameScene&& other) = delete;
        GameScene& operator=(const GameScene& rhs) = delete;
        GameScene& operator=(GameScene&& rhs) = delete;
        ~GameScene();

        Entity& CreateEntity();
        void GameScene::OnAwake();
        void OnUpdate(float timestep);
        void VisitEntities(std::function<void(Entity)> callback);
        const SceneRenderData& GetRenderData();
        inline void SetAmbientLight(float radiance)
        {
            m_render_data.ambient_light_radiance = radiance;
        }

    private:
        friend class Entity;
    };
}  // namespace Merlin

#endif