#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <functional>
#include <vector>

#include "Merlin/Scene/game_scene.hpp"

namespace Merlin
{
    class Entity
    {
        entt::entity m_entity_handle;
        GameScene* m_scene;

    public:
        Entity(entt::entity entity_handle, GameScene* scene)
            : m_entity_handle(entity_handle), m_scene(scene)
        {
        }

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            return m_scene->m_registry.emplace<T>(m_entity_handle,
                                                  std::forward<Args>(args)...);
        }

        template <typename T>
        T& GetComponent()
        {
            return m_scene->m_registry.get<T>(m_entity_handle);
        }
    };
}  // namespace Merlin

#endif `