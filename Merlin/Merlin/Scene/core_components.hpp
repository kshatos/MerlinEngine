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

    class MeshRenderComponent : public Component
    {
    public:
        MeshRenderComponent(Entity* parent) : Component(parent) {}
        std::shared_ptr<VertexArray> varray;
        std::shared_ptr<Shader> shader;
    };

    class PointLightComponent : public Component
    {
        std::shared_ptr<TransformComponent> transform;
    public:
        PointLightData data;

        PointLightComponent(Entity* parent) : Component(parent) {}

        void OnAwake() override
        {
            transform = parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            data.position = transform->transform.GetPosition();
        }
    };

    class DirectionalLightComponent : public Component
    {
    public:
        DirectionalLightComponent(Entity* parent) : Component(parent) {}
        DirectionalLightData data;
    };
}

#endif