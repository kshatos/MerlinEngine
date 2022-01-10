#ifndef SCENE_RENDER_DATA_HPP
#define SCENE_RENDER_DATA_HPP
#include <vector>
#include "Merlin/Render/skybox.hpp"
#include "Merlin/Render/camera.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/material.hpp"
#include "Merlin/Render/frame_buffer.hpp"


namespace Merlin
{
    struct CameraRenderData
    {
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Skybox> skybox;
        std::shared_ptr<FrameBuffer> frame_buffer;
        glm::vec4 clear_color;
        glm::vec3 view_pos;
        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;
    };

    struct PointLightData
    {
        glm::vec3 position;
        float radiantFlux;
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
        float cutoffAngle;
        float falloffRatio;
        float radiantIntensity;
        float range;
        glm::vec3 color;
    };

    struct MeshRenderData
    {
        std::shared_ptr<VertexArray> vertex_array;
        std::shared_ptr<Material> material;
        glm::mat4 model_matrix;

        MeshRenderData() :
            material(nullptr),
            vertex_array(nullptr),
            model_matrix(glm::mat4(0.0)) {}

        MeshRenderData(
            std::shared_ptr<Material> _material,
            std::shared_ptr<VertexArray> _vertex_array,
            glm::mat4 _model_matrix)
            :
            material(_material),
            vertex_array(_vertex_array),
            model_matrix(_model_matrix) {}
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

}
#endif