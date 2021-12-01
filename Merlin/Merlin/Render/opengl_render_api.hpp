#ifndef OPENGL_RENDER_API_HPP
#define OPENGL_RENDER_API_HPP
#include "Merlin/Render/render_api.hpp"


namespace Merlin
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        void SetClearColor(const glm::uvec4& color) override;
        void Clear() override;
        void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) override;
    };
}

#endif