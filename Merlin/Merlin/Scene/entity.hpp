#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <vector>
#include"Merlin/Scene/component.hpp"


namespace Merlin
{
    class Entity
    {
        std::vector<std::shared_ptr<Component>> components;
    public:
        template<typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args)
        {
            auto component = std::shared_ptr<T>(new T(this, std::forward<Args>(args)...));
            components.push_back(component);
            return component;
        }

        template<typename T>
        std::shared_ptr<T> GetComponent()
        {
            for(auto & component : components)
            {
                auto componentT = std::dynamic_pointer_cast<T>(component);
                if (componentT != nullptr)
                    return componentT;
            }
            return nullptr;
        }

        void OnAwake();
        void OnUpdate(float time_step);
    };
}

#endif`