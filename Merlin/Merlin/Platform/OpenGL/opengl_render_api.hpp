#ifndef OPENGL_RENDER_API_HPP
#define OPENGL_RENDER_API_HPP
#include "Merlin/Render/render_api.hpp"


namespace Merlin
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        void Init() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) override;
    };
}

#endif