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
        struct SceneData
        {
            glm::vec3 view_pos;
            glm::mat4 view_matrix;
            glm::mat4 projection_matrix;
            std::vector<PointLightData> point_lights;
            std::vector<DirectionalLightData> directional_lights;
            std::vector<SpotLightData> spot_lights;
            float ambient_radiance;
        };

        static std::unique_ptr <SceneData> m_scene_data;
        static std::unique_ptr<RenderAPI> m_render_impl;
    public:
        static void Init();
        static void BeginScene(const std::shared_ptr<Camera>& camera);
        static void AddLight(const PointLightData& light);
        static void AddLight(const DirectionalLightData& light);
        static void AddLight(const SpotLightData& light);
        static void SetAmbientLighting(float ambient_radiance);
        static void EndScene();

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();
        static void Submit(const std::shared_ptr<Skybox>& skybox);
        static void Renderer::Submit(
            const std::shared_ptr<Material>& material,
            const std::shared_ptr<VertexArray>& vertex_array,
            const glm::mat4& model_matrix);
    };
}

#endif