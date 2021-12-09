#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <memory>
#include "Merlin/Render/transform.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/shader.hpp"


namespace Merlin
{
    class Entity;

    class Component
    {
    protected:
        Entity* parent;
    public:
        Component(Entity* _parent) { parent = _parent; }
        virtual ~Component() {}
        virtual void OnAwake() {};
        virtual void OnUpdate(float time_step) {}
    };

    class TransformComponent : public Component
    {
    public:
        TransformComponent(Entity* parent) : Component(parent) {}
        Transform transform;
    };

    class MeshRenderComponent : public Component
    {
    public:
        MeshRenderComponent(Entity* parent) : Component(parent) {}
        std::shared_ptr<VertexArray> varray;
        std::shared_ptr<Shader> shader;
    };
}

#endif
