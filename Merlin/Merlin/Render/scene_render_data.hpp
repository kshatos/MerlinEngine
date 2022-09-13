#ifndef SCENE_RENDER_DATA_HPP
#define SCENE_RENDER_DATA_HPP
#include <vector>

#include "Merlin/Render/camera_projection.hpp"
#include "Merlin/Render/frame_buffer.hpp"
#include "Merlin/Render/material.hpp"
#include "Merlin/Render/mesh_buffer.hpp"
#include "Merlin/Render/skybox.hpp"

namespace Merlin
{
    struct CameraRenderData
    {
        std::shared_ptr<CameraProjection> camera;
        std::shared_ptr<Skybox> skybox;
        std::shared_ptr<FrameBuffer> frame_buffer;
        glm::vec4 clear_color;
        glm::vec3 view_pos;
        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;

        CameraRenderData()
            : camera(nullptr)
            , skybox(nullptr)
            , frame_buffer(nullptr)
            , clear_color()
            , view_pos()
            , view_matrix()
            , projection_matrix()
        {
        }
    };

    struct PointLightData
    {
        glm::vec3 position;
        float radiant_flux;
        float range;
        glm::vec3 color;
    };

    struct DirectionalLightData
    {
        glm::vec3 direction;
        float irradiance;
        glm::vec3 color;
    };

    struct SpotLightData
    {
        glm::vec3 position;
        glm::vec3 direction;
        float cutoff_angle;
        float falloff_ratio;
        float radiant_intensity;
        float range;
        glm::vec3 color;
    };

    struct MeshRenderData
    {
        std::shared_ptr<MeshBuffer> mesh_buffer;
        std::shared_ptr<MaterialInstance> material_instance;
        glm::mat4 model_matrix;

        MeshRenderData()
            : material_instance(nullptr)
            , mesh_buffer(nullptr)
            , model_matrix(glm::mat4(0.0))
        {
        }

        MeshRenderData(std::shared_ptr<MaterialInstance> _material_instance,
                       std::shared_ptr<MeshBuffer> _mesh_buffer,
                       glm::mat4 _model_matrix)
            : material_instance(_material_instance)
            , mesh_buffer(_mesh_buffer)
            , model_matrix(_model_matrix)
        {
        }
    };

    struct SceneRenderData
    {
        CameraRenderData* camera;
        float ambient_light_radiance;
        std::vector<PointLightData*> point_lights;
        std::vector<DirectionalLightData*> directional_lights;
        std::vector<SpotLightData*> spot_lights;
        std::vector<MeshRenderData*> meshes;
    };

}  // namespace Merlin
#endif