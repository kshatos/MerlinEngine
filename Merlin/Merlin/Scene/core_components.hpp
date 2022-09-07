#ifndef CORE_COMPONENTS_HPP
#define CORE_COMPONENTS_HPP
#include <optional>

#include "Merlin/Render/scene_render_data.hpp"
#include "Merlin/Scene/entity.hpp"

namespace Merlin
{
    struct EntityInfoComponent
    {
        std::string name;

        EntityInfoComponent() : name("New Entity") {}
    };

    struct EntityTreeComponent
    {
        std::optional<Entity> parent;
        std::set<Entity> children;
    };

    struct TransformComponent
    {
        Transform transform;
    };

    struct CameraComponent
    {
        CameraRenderData camera_data;
    };

    struct MeshRenderComponent
    {
        MeshRenderData m_data;
    };

    struct PointLightComponent
    {
        PointLightData m_data;
    };

    struct DirectionalLightComponent
    {
        DirectionalLightData m_data;
    };

    struct SpotLightComponent
    {
        SpotLightData m_data;
    };
}  // namespace Merlin

#endif