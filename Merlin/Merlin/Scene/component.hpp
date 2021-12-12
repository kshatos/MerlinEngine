#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <memory>
#include "Merlin/Render/transform.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/lighting.hpp"


namespace Merlin
{
    class Entity;

    class Component
    {
    protected:
        Entity* m_parent;
    public:
        Component(Entity* parent) { m_parent = parent; }
        virtual ~Component() {}
        virtual void OnAwake() {};
        virtual void OnUpdate(float time_step) {}
    };
}

#endif
