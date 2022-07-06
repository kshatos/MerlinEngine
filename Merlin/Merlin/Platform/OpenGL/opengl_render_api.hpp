#ifndef OPENGL_RENDER_API_HPP
#define OPENGL_RENDER_API_HPP
#include "Merlin/Render/render_api.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>


namespace Merlin
{
    class OpenGLRenderAPI : public RenderAPI
    {
        GLFWwindow* m_window;
        ImGuiContext* context;
    public:
        void Init(void* windowPointer) override;
        void Shutdown() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) override;
        void SwapBuffers() override;
        RenderBackend Backend() override;

        std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices, size_t size) override;
        std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t index_count) override;
        std::shared_ptr<VertexArray> CreateVertexArray() override;
        std::shared_ptr<Shader> CreateShader(
            const std::string& vertex_source,
            const std::string& fragment_source) override;
        std::shared_ptr<Texture2D> CreateTexture2D(
            const std::string& filepath,
            Texture2DProperties props = Texture2DProperties()) override;
        std::shared_ptr<Texture2D> CreateTexture2D(
            void* data,
            uint32_t width,
            uint32_t height,
            uint32_t channel_count,
            Texture2DProperties props = Texture2DProperties()) override;
    };
}

#endif