#ifndef CORE_COMPONENTS_HPP
#define CORE_COMPONENTS_HPP
#include "Merlin/Scene/entity.hpp"
#include "Merlin/Render/scene_render_data.hpp"

namespace Merlin
{

    class TransformComponent : public Component
    {
    public:
        TransformComponent(Entity* parent) : Component(parent) {}
        Transform transform;
    };

    class CameraComponent : public Component
    {
        std::shared_ptr<TransformComponent> m_transform;
    public:
        CameraComponent(Entity* parent) : Component(parent) {}
        CameraRenderData data;
        
        void OnAwake() override
        {
            m_transform = m_parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            data.projection_matrix = data.camera->GetProjectionMatrix();
            data.view_matrix = glm::inverse(m_transform->transform.GetTransformationMatrix());
            data.view_pos = m_transform->transform.GetPosition();
        }
    };

    class MeshRenderComponent : public Component
    {
        std::shared_ptr<TransformComponent> m_transform;

    public:
        MeshRenderComponent(Entity* parent) : Component(parent) {}
        MeshRenderData data;

        void OnAwake() override
        {
            m_transform = m_parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            data.model_matrix = m_transform->transform.GetTransformationMatrix();
        }
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