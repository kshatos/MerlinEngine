#include "Merlin/Scene/entity.hpp"

namespace Merlin
{

    bool Entity::operator==(Entity const& other)
    {
        return (m_scene == other.m_scene &&
                m_entity_handle == other.m_entity_handle);
    }
}  // namespace Merlin