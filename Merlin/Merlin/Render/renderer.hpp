#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Merlin/Render/camera.hpp"
#include "Merlin/Render/lighting.hpp"
#include "Merlin/Render/skybox.hpp"


namespace Merlin
{
    class RenderAPI;
    class VertexArray;
    class Shader;
    class Material;

    class Renderer
    {
        struct MeshRenderData
        {
            std::shared_ptr<Material> material;
            std::shared_ptr<VertexArray> vertex_array;
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

        struct SceneData
        {
            glm::vec3 view_pos;
            glm::mat4 view_matrix;
            glm::mat4 projection_matrix;
            std::vector<PointLightData> point_lights;
            std::vector<DirectionalLightData> directional_lights;
            std::vector<SpotLightData> spot_lights;
            std::shared_ptr<Skybox> skybox;
            std::vector<MeshRenderData> mesh_render_data;
            float ambient_radiance;
        };

        static std::unique_ptr <SceneData> m_scene_data;
        static std::unique_ptr<RenderAPI> m_render_impl;
    public:
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();

        static void Init();
        static void BeginScene(const std::shared_ptr<Camera>& camera);
        static void AddLight(const PointLightData& light);
        static void AddLight(const DirectionalLightData& light);
        static void AddLight(const SpotLightData& light);
        static void SetAmbientLighting(float ambient_radiance);
        static void SetSkybox(const std::shared_ptr<Skybox>& skybox);
        static void Submit(
            const std::shared_ptr<Material>& material,
            const std::shared_ptr<VertexArray>& vertex_array,
            const glm::mat4& model_matrix);
        static void EndScene();
    private:
        static void Draw(const std::shared_ptr<Skybox>& skybox);
        static void Draw(const MeshRenderData& data);
    };
}

#endif