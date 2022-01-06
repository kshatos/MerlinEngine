#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Merlin/Render/camera.hpp"
#include "Merlin/Render/skybox.hpp"
#include "Merlin/Render/scene_render_data.hpp"


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
            CameraRenderData camera;
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
        static void BeginScene(const CameraRenderData& camera);
        static void AddLight(const PointLightData& light);
        static void AddLight(const DirectionalLightData& light);
        static void AddLight(const SpotLightData& light);
        static void SetAmbientLighting(float ambient_radiance);
        static void SetSkybox(const std::shared_ptr<Skybox>& skybox);
        static void Submit(const MeshRenderData& data);
        static void EndScene();
    private:
        static void Draw(const std::shared_ptr<Skybox>& skybox);
        static void Draw(const MeshRenderData& data);
    };
}

#endif