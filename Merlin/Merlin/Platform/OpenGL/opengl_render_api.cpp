#include <glad/glad.h>
#include "Merlin/Platform/OpenGL/opengl_render_api.hpp"


namespace Merlin
{
    void OpenGLRenderAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
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
}
