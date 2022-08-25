#include "Merlin/Scene/entity.hpp"

namespace Merlin
{
    void Entity::OnAwake()
    {
        for (auto& component : m_components)
        {
            component->OnAwake();
        }
    }

    void Entity::OnUpdate(float time_step)
    {
        for (auto& component : m_components)
        {
            component->OnUpdate(time_step);
        }
    }
}  // namespace Merlin