#include "Merlin/Scene/entity.hpp"

#include "Merlin/Scene/game_scene.hpp"

namespace Merlin
{

    bool Entity::operator==(Entity const& other)
    {
        return (m_scene == other.m_scene &&
                m_entity_handle == other.m_entity_handle);
    }

    bool operator<(Entity const& left, Entity const& right)
    {
        return left.GetID() < right.GetID();
    }

    void Entity::Destroy()
    {
        // Remove parent child references
        auto& tree_component = GetComponent<EntityTreeComponent>();
        if (tree_component.parent.has_value())
        {
            auto& parent_tree =
                tree_component.parent->GetComponent<EntityTreeComponent>();
            parent_tree.children.erase(*this);
        }
        for (auto& child : tree_component.children)
        {
            auto& child_tree = child.GetComponent<EntityTreeComponent>();
            child_tree.parent.reset();
        }

        m_scene->m_registry.destroy(m_entity_handle);
    }

}  // namespace Merlin