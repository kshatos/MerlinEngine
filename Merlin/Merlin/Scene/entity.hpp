#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <entt/entt.hpp>
#include <functional>
#include <vector>
#include "Merlin/Core/uuid.hpp"

namespace Merlin
{
    class GameScene;

    class Entity
    {
        entt::entity m_entity_handle;
        GameScene* m_scene;

    public:
        Entity(entt::entity entity_handle, GameScene* scene)
            : m_entity_handle(entity_handle), m_scene(scene)
        {
        }

        template <typename T>
        bool HasComponent()
        {
            return m_scene->m_registry.any_of<T>(m_entity_handle);
        }

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            if (HasComponent<T>()) return GetComponent<T>();
            return m_scene->m_registry.emplace<T>(m_entity_handle,
                                                  std::forward<Args>(args)...);
        }

        template <typename T>
        void RemoveComponent()
        {
            if (!HasComponent<T>()) return;
            m_scene->m_registry.remove<T>(m_entity_handle);
        }

        void AddChild(Entity child);
        void RemoveChild(Entity child);
        void RemoveParent();
        bool IsAncestorOf(Entity entity);

        void Destroy();

        template <typename T>
        T& GetComponent() const
        {
            return m_scene->m_registry.get<T>(m_entity_handle);
        }

        inline uint32_t GetID() const
        {
            return static_cast<uint32_t>(m_entity_handle);
        }

        UUID GetUUID() const;

        bool operator==(Entity const& other);
    };

    bool operator<(Entity const& left, Entity const& right);
}  // namespace Merlin

#endif `