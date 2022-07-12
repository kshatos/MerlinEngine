#include "Merlin/Render/renderer.hpp"
#include "Merlin/Render/render_api.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/material.hpp"


namespace Merlin
{
    std::shared_ptr<RenderAPI> Renderer::m_render_impl = nullptr;

    void Renderer::Init(std::shared_ptr<RenderAPI> renderApi)
    {
        m_render_impl = renderApi;
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        m_render_impl->SetClearColor(color);
    }

    void Renderer::Clear()
    {
        m_render_impl->Clear();
    }

    void Renderer::RenderScene(const SceneRenderData& scene)
    {
        m_render_impl->RenderScene(scene);
    }

    std::shared_ptr<RenderAPI> Renderer::GetAPI()
    {
        return m_render_impl;
    }

}