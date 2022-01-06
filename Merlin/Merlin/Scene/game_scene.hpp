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
        SceneRenderData render_data;
        std::shared_ptr<CameraComponent> m_camera;
        std::shared_ptr<Skybox> m_skybox;
        std::vector<std::shared_ptr<Entity>> m_entities;
    public:
        inline void SetCamera(std::shared_ptr<CameraComponent> camera) { m_camera = camera; }
        inline void SetSkybox(std::shared_ptr<Skybox> skybox) { m_skybox = skybox; }
        std::shared_ptr<Entity> CreateEntity();
        void RenderScene();
        void GameScene::OnAwake();
        void OnUpdate(float timestep);
    private:
        void OnComponentAdded(std::shared_ptr<Component> component);
    };
}

#endif