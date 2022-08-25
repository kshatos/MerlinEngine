#ifndef CORE_COMPONENTS_HPP
#define CORE_COMPONENTS_HPP
#include "Merlin/Render/scene_render_data.hpp"
#include "Merlin/Scene/entity.hpp"

namespace Merlin
{

    class TransformComponent : public Component
    {
    public:
        TransformComponent(Entity* parent) : Component(parent) {}
        Transform m_transform;
    };

    class CameraComponent : public Component
    {
        std::shared_ptr<TransformComponent> m_transform;

    public:
        CameraComponent(Entity* parent) : Component(parent) {}
        CameraRenderData m_data;

        void OnAwake() override
        {
            m_transform = m_parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            m_data.projection_matrix = m_data.camera->GetProjectionMatrix();
            m_data.view_matrix = glm::inverse(
                m_transform->m_transform.GetTransformationMatrix());
            m_data.view_pos = m_transform->m_transform.GetPosition();
        }
    };

    class MeshRenderComponent : public Component
    {
        std::shared_ptr<TransformComponent> m_transform;

    public:
        MeshRenderComponent(Entity* parent) : Component(parent) {}
        MeshRenderData m_data;

        void OnAwake() override
        {
            m_transform = m_parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            m_data.model_matrix =
                m_transform->m_transform.GetTransformationMatrix();
        }
    };

    class PointLightComponent : public Component
    {
        std::shared_ptr<TransformComponent> m_transform;

    public:
        PointLightData m_data;

        PointLightComponent(Entity* parent) : Component(parent), m_data() {}

        void OnAwake() override
        {
            m_transform = m_parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            m_data.position = m_transform->m_transform.GetPosition();
        }
    };

    class DirectionalLightComponent : public Component
    {
    public:
        DirectionalLightData m_data;

        DirectionalLightComponent(Entity* parent) : Component(parent), m_data()
        {
        }
    };

    class SpotLightComponent : public Component
    {
        std::shared_ptr<TransformComponent> m_transform;

    public:
        SpotLightData m_data;

        SpotLightComponent(Entity* parent) : Component(parent), m_data() {}

        void OnAwake() override
        {
            m_transform = m_parent->GetComponent<TransformComponent>();
        }

        void OnUpdate(float time_step) override
        {
            m_data.position = m_transform->m_transform.GetPosition();
            m_data.direction = m_transform->m_transform.GetOrientation() *
                             glm::vec3(0.0, 0.0, -1.0);
        }
    };
}  // namespace Merlin

#endif