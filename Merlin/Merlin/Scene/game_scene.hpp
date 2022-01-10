#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP
#include <memory>
#include "Merlin/Scene/entity.hpp"
#include "Merlin/Render/camera.hpp"
#include "Merlin/Render/skybox.hpp"
#include "Merlin/Render/scene_render_data.hpp"
#include "Merlin/Scene/core_components.hpp"


namespace Merlin
{
    class GameScene
    {
        std::vector<std::shared_ptr<Entity>> m_entities;
        SceneRenderData m_render_data;
    public:
        std::shared_ptr<Entity> CreateEntity();
        void RenderScene();
        void GameScene::OnAwake();
        void OnUpdate(float timestep);
        inline void SetAmbientLight(float radiance) { m_render_data.ambient_light_radiance = radiance; }
    private:
        void OnComponentAdded(std::shared_ptr<Component> component);
    };
}

#endif