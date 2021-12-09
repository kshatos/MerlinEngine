#include "Merlin/Scene/entity.hpp"


namespace Merlin
{
    void Entity::OnAwake()
    {
        for(auto & component : components)
        {
            component->OnAwake();
        }
    }

    void Entity::OnUpdate(float time_step)
    {
        for (auto& component : components)
        {
            component->OnUpdate(time_step);
        }
    }
}