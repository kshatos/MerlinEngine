#include <glad/glad.h>
#include "Merlin/Platform/OpenGL/opengl_render_api.hpp"
#include "Merlin/Core/logger.hpp"
#include <backends/imgui_impl_opengl3.h>
#include "backends/imgui_impl_glfw.h"
#include "Merlin/Platform/OpenGL/opengl_vertex_buffer.hpp"
#include "Merlin/Platform/OpenGL/opengl_index_buffer.hpp"
#include "Merlin/Platform/OpenGL/opengl_vertex_array.hpp"
#include "Merlin/Platform/OpenGL/opengl_shader.hpp"
#include "Merlin/Platform/OpenGL/opengl_texture2d.hpp"


namespace Merlin
{
    void GLAPIENTRY
        LogOpenGLErrors(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
    {
        ME_LOG_INFO(std::string(message));
    }

    void OpenGLRenderAPI::Init(void* windowPointer)
    {
        m_window = (GLFWwindow*)windowPointer;

        glfwMakeContextCurrent(m_window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        IMGUI_CHECKVERSION();
        context = ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)windowPointer, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(LogOpenGLErrors, 0);
    }

    void OpenGLRenderAPI::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(context);
    }

    void OpenGLRenderAPI::SetViewport(
        uint32_t x,
        uint32_t y,
        uint32_t width,
        uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderAPI::DrawTriangles(
        const std::shared_ptr<VertexArray>& vertex_array)
    {
        glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    RenderBackend OpenGLRenderAPI::Backend()
    {
        return RenderBackend::OPENGL;
    }

    void OpenGLRenderAPI::SwapBuffers()
    {
        glfwSwapBuffers(m_window);;
    }


    std::shared_ptr<VertexBuffer> OpenGLRenderAPI::CreateVertexBuffer(
        float* vertices, size_t size)
    {
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    std::shared_ptr<IndexBuffer> OpenGLRenderAPI::CreateIndexBuffer(
        uint32_t* indices, uint32_t index_count)
    {
        return std::make_shared<OpenGLIndexBuffer>(indices, index_count);
    }

    std::shared_ptr<VertexArray> OpenGLRenderAPI::CreateVertexArray()
    {
        return std::make_shared<OpenGLVertexArray>();
    }

    std::shared_ptr<Shader> OpenGLRenderAPI::CreateShader(
        const std::string& vertex_source,
        const std::string& fragment_source)
    {
        return std::shared_ptr<OpenGLShader>(
            OpenGLShader::CreateFromFiles(
                vertex_source, fragment_source));
    }

    std::shared_ptr<Texture2D> OpenGLRenderAPI::CreateTexture2D(
        const std::string& filepath,
        Texture2DProperties props)
    {
        return std::make_shared<OpenGLTexture2D>(
            filepath, props);
    }

    std::shared_ptr<Texture2D> OpenGLRenderAPI::CreateTexture2D(
        void* data,
        uint32_t width,
        uint32_t height,
        uint32_t channel_count,
        Texture2DProperties props)
    {
        return std::make_shared<OpenGLTexture2D>(
            data, width, height, channel_count, props);
    }

}
