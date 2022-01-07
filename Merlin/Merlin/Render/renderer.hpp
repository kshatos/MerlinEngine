#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Merlin/Render/scene_render_data.hpp"


namespace Merlin
{
    class RenderAPI;

    class Renderer
    {
        static std::shared_ptr<Shader> m_shadow_shader;
        static std::unique_ptr<RenderAPI> m_render_impl;
    public:
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();

        static void Init();
        static void RenderScene(const SceneRenderData& scene);

    private:
        static void DrawMeshes(const SceneRenderData& scene);
        static void DrawSkybox(const SceneRenderData& scene);
    };
}

#endif