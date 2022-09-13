#ifndef CORE_COMPONENTS_HPP
#define CORE_COMPONENTS_HPP
#include <optional>

#include "Merlin/Core/uuid.hpp"
#include "Merlin/Render/scene_render_data.hpp"
#include "Merlin/Scene/entity.hpp"

namespace Merlin
{
    struct EntityInfoComponent
    {
        std::string name;
        const UUID uuid;

        EntityInfoComponent(UUID _uuid = UUID())
            : name("New Entity"), uuid(_uuid.IsNil() ? UUID() : _uuid)
        {
        }
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
        glm::vec4 clear_color;
        std::shared_ptr<CameraProjection> projection;
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