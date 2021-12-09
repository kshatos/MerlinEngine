#ifndef SCENE_HPP
#define SCENE_HPP
#include <memory>
#include "Merlin/Scene/entity.hpp"
#include "Merlin/Render/camera.hpp"


namespace Merlin
{
    class Scene
    {
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<Entity>> entities;
    public:
        inline void SetCamera(std::shared_ptr<Camera> _camera) { camera = _camera; }
        inline void AddEntity(std::shared_ptr<Entity> _entity) { entities.push_back(_entity); }
        void RenderScene();
        void Scene::OnAwake();
        void OnUpdate(float timestep);
    };
}

#endif