#ifndef RENDER_API_HPP
#define RENDER_API_HPP
#include <glm/glm.hpp>
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/skybox.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/texture2d.hpp"


namespace Merlin
{
    enum class RenderBackend
    {
        OPENGL,
        VULKAN
    };

    class RenderAPI
    {
    public:
        virtual ~RenderAPI() {}
        virtual void Init(void* windowPointer) = 0;
        virtual void Shutdown() = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) = 0;
        virtual void SwapBuffers() = 0;
        virtual RenderBackend Backend() = 0;

        virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(
            float* vertices, size_t size) = 0;
        virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(
            uint32_t* indices, uint32_t index_count) = 0;
        virtual std::shared_ptr<VertexArray> CreateVertexArray() = 0;
        virtual std::shared_ptr<Shader> CreateShader(
            const std::string& vertex_source,
            const std::string& fragment_source) = 0;
        virtual std::shared_ptr<Texture2D> CreateTexture2D(
            const std::string& filepath,
            Texture2DProperties props = Texture2DProperties())=0;
        virtual std::shared_ptr<Texture2D> CreateTexture2D(
            void* data,
            uint32_t width,
            uint32_t height,
            uint32_t channel_count,
            Texture2DProperties props = Texture2DProperties())=0;

        static std::shared_ptr<RenderAPI> Create(RenderBackend backend);
    };
}

#endif