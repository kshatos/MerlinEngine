#ifndef CORE_COMPONENTS_HPP
#define CORE_COMPONENTS_HPP
#include "Merlin/Scene/entity.hpp"


namespace Merlin
{

    class TransformComponent : public Component
    {
    public:
        TransformComponent(Entity* parent) : Component(parent) {}
        Transform transform;
    };

    class Material;
    class MeshRenderComponent : public Component
    {
    public:
        MeshRenderComponent(Entity* parent) : Component(parent) {}
        std::shared_ptr<VertexArray> varray;
        std::shared_ptr<Material> material;
    };

    class PointLightComponent : public Component
    {
        std::shared_ptr<TransformComponent> m_transform;
    public:
        PointLightData data;

        PointLightComponent(Entity* parent) : Component(parent) {}

        void OnAwake() override
        {
            m_transform = m_parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            data.position = m_transform->transform.GetPosition();
        }
    };

    class DirectionalLightComponent : public Component
    {
    public:
        DirectionalLightData data;

        DirectionalLightComponent(Entity* parent) : Component(parent) {}
    };

    class SpotLightComponent : public Component
    {
        std::shared_ptr<TransformComponent> m_transform;
    public:
        SpotLightData data;

        SpotLightComponent(Entity* parent) : Component(parent) {}

        void OnAwake() override
        {
            m_transform = m_parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            data.position = m_transform->transform.GetPosition();
            data.direction = m_transform->transform.GetOrientation() * glm::vec3(0.0, 0.0, -1.0);
        }
    };
}

#endif