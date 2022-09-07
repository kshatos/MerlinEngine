#include "Merlin/Scene/entity.hpp"

#include <queue>

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

    void Entity::AddChild(Entity child)
    {
        auto& tree_comp = GetComponent<EntityTreeComponent>();
        auto& child_tree = child.GetComponent<EntityTreeComponent>();

        tree_comp.children.insert(child);
        child_tree.parent = *this;
    }

    void Entity::RemoveChild(Entity child)
    {
        auto& tree_comp = GetComponent<EntityTreeComponent>();
        auto& child_tree = child.GetComponent<EntityTreeComponent>();

        tree_comp.children.erase(child);
        child_tree.parent.reset();
    }

    void Entity::RemoveParent()
    {
        auto& tree_comp = GetComponent<EntityTreeComponent>();
        if (!tree_comp.parent.has_value()) return;

        auto& parent_tree =
            tree_comp.parent->GetComponent<EntityTreeComponent>();
        parent_tree.children.erase(*this);
        tree_comp.parent.reset();
    }

    bool Entity::IsAncestorOf(Entity entity)
    {
        auto& tree_component = GetComponent<EntityTreeComponent>();
        std::queue<Entity> search_queue;
        search_queue.push(*this);
        while (!search_queue.empty())
        {
            auto search_entity = search_queue.front();
            search_queue.pop();

            if (search_entity == entity) return true;

            auto& search_tree =
                search_entity.GetComponent<EntityTreeComponent>();
            for (auto& child : search_tree.children) search_queue.push(child);
        }
        return false;
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