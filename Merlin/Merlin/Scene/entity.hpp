#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <functional>
#include <vector>

#include "Merlin/Scene/component.hpp"

namespace Merlin
{
    class Entity
    {
        std::vector<std::shared_ptr<Component>> m_components;

    public:
        template <typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args)
        {
            auto component =
                std::shared_ptr<T>(new T(this, std::forward<Args>(args)...));
            component_added_callback(component);
            m_components.push_back(component);
            return component;
        }

        template <typename T>
        std::shared_ptr<T> GetComponent()
        {
            for (auto& component : m_components)
            {
                auto typed_component = std::dynamic_pointer_cast<T>(component);
                if (typed_component != nullptr) return typed_component;
            }
            return nullptr;
        }

        void OnAwake();
        void OnUpdate(float time_step);

    private:
        friend class GameScene;
        Entity(){};
        std::function<void(std::shared_ptr<Component> component)>
            component_added_callback;
    };
}  // namespace Merlin

#endif `