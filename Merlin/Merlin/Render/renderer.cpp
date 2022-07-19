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

    std::shared_ptr<VertexBuffer> Renderer::Renderer::CreateVertexBuffer(
        float* vertices, size_t size)
    {
        return m_render_impl->CreateVertexBuffer(vertices, size);
    }

    std::shared_ptr<IndexBuffer> Renderer::CreateIndexBuffer(
        uint32_t* indices, uint32_t index_count)
    {
        return m_render_impl->CreateIndexBuffer(indices, index_count);
    }

    std::shared_ptr<VertexArray> Renderer::CreateVertexArray()
    {
        return m_render_impl->CreateVertexArray();
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

    std::shared_ptr<Shader> Renderer::CreateShader(
        const std::string& vertex_source,
        const std::string& fragment_source)
    {
        return m_render_impl->CreateShader(vertex_source, fragment_source);
    }

    std::shared_ptr<Texture2D> Renderer::CreateTexture2D(
        const std::string& filepath,
        Texture2DProperties props)
    {
        return m_render_impl->CreateTexture2D(filepath, props);
    }

    std::shared_ptr<Texture2D> Renderer::CreateTexture2D(
        void* data,
        uint32_t width,
        uint32_t height,
        uint32_t channel_count,
        Texture2DProperties props)
    {
        return m_render_impl->CreateTexture2D(data, width, height, channel_count, props);
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