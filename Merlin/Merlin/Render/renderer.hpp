#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Merlin/Render/scene_render_data.hpp"
#include "Merlin/Render/render_api.hpp"


namespace Merlin
{
    class Renderer
    {
        static std::shared_ptr<RenderAPI> m_render_impl;
    public:
        static void Init(std::shared_ptr<RenderAPI> renderApi);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();
        static void RenderScene(const SceneRenderData& scene);
        static std::shared_ptr<RenderAPI> GetAPI();
    };
}

#endif