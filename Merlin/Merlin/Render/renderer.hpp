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
        static void RenderScene(const SceneRenderData& scene);

        static std::shared_ptr<MeshBuffer> CreateMeshBuffer(
            float* vertices,
            size_t vertex_count,
            uint32_t* indices,
            size_t index_count,
            BufferLayout vertexLayout);
        static std::shared_ptr<Shader> CreateShader(
            const std::string& vertex_source,
            const std::string& fragment_source);
        static std::shared_ptr<Texture2D> CreateTexture2D(
            const std::string& filepath,
            Texture2DProperties props = Texture2DProperties());
        static std::shared_ptr<Texture2D> CreateTexture2D(
            void* data,
            uint32_t width,
            uint32_t height,
            uint32_t channel_count,
            Texture2DProperties props = Texture2DProperties());
        static std::shared_ptr<Cubemap> CreateCubemap(
            const std::vector<std::string>& face_paths);
        static std::shared_ptr<Cubemap> CreateCubemap(
            uint32_t resolution, uint32_t channel_count);
        static std::shared_ptr<FrameBuffer> CreateFramebuffer(
            const FrameBufferParameters& state);
    };
}

#endif