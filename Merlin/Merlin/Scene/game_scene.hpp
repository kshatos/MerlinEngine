#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP
#include <memory>
#include "Merlin/Scene/entity.hpp"
#include "Merlin/Render/camera.hpp"


namespace Merlin
{
    class GameScene
    {
        std::shared_ptr<Camera> m_camera;
        std::vector<std::shared_ptr<Entity>> m_entities;
    public:
        inline void SetCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }
        inline void AddEntity(std::shared_ptr<Entity> entity) { m_entities.push_back(entity); }
        void RenderScene();
        void GameScene::OnAwake();
        void OnUpdate(float timestep);
    };
}

#endif