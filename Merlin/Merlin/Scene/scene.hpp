#ifndef SCENE_HPP
#define SCENE_HPP
#include <memory>
#include "Merlin/Render/camera.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/transform.hpp"


namespace Merlin
{
    class Entity
    {
    public:
        Transform transform;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexArray> varray;

        virtual void OnBeforeRender() {}
        virtual void OnUpdate(float time_step) {}
    };

    class Scene
    {
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<Entity>> entities;
    public:
        inline void SetCamera(std::shared_ptr<Camera> _camera) { camera = _camera; }
        inline void AddEntity(std::shared_ptr<Entity> _entity) { entities.push_back(_entity); }
        void RenderScene();
        void OnUpdate(float timestep);
    };
}

#endif