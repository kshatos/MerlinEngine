#ifndef RENDER_API_HPP
#define RENDER_API_HPP
#include <glm/glm.hpp>
#include "Merlin/Render/vertex_array.hpp"


namespace Merlin
{
    class RenderAPI
    {
    public:
        virtual ~RenderAPI() {}
        virtual void SetClearColor(const glm::uvec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) = 0;

        static RenderAPI* Create();
    };
}

#endif