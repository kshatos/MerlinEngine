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

    void Renderer::RenderScene(const SceneRenderData& scene)
    {
        m_render_impl->RenderScene(scene);
    }

    std::shared_ptr<MeshBuffer> Renderer::CreateMeshBuffer(
        float* vertices,
        size_t vertex_count,
        uint32_t* indices,
        size_t index_count,
        BufferLayout vertexLayout)
    {
        return m_render_impl->CreateMeshBuffer(
            vertices, vertex_count, indices, index_count, vertexLayout);
    }

    std::shared_ptr<Material> Renderer::CreateMaterial(
        MaterialInfo info)
    {
        return m_render_impl->CreateMaterial(info);
    }

    std::shared_ptr<MaterialInstance> Renderer::CreateMaterialInstance(
        std::shared_ptr<Material> material)
    {
        return m_render_impl->CreateMaterialInstance(material);
    }

    std::shared_ptr<Shader> Renderer::CreateShader(
        const std::string& vertex_source,
        const std::string& fragment_source)
    {
        return m_render_impl->CreateShader(vertex_source, fragment_source);
    }

    std::shared_ptr<Texture2D> Renderer::CreateTexture2D(
        const Texture2DData& texture_data,
        Texture2DProperties props)
    {
        return m_render_impl->CreateTexture2D(texture_data, props);
    }

    std::shared_ptr<Cubemap> Renderer::CreateCubemap(
        const std::vector<std::string>& face_paths)
    {
        return m_render_impl->CreateCubemap(face_paths);
    }

    std::shared_ptr<Cubemap> Renderer::CreateCubemap(
        uint32_t resolution, uint32_t channel_count)
    {
        return m_render_impl->CreateCubemap(resolution, channel_count);
    }

    std::shared_ptr<FrameBuffer> Renderer::CreateFramebuffer(
        const FrameBufferParameters& state)
    {
        return m_render_impl->CreateFramebuffer(state);
    }

}