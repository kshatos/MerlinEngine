#ifndef RENDER_API_HPP
#define RENDER_API_HPP
#include <glm/glm.hpp>
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/skybox.hpp"


namespace Merlin
{
    class RenderAPI
    {
    public:
        virtual ~RenderAPI() {}
        virtual void Init()=0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) = 0;

        static RenderAPI* Create();
    };
}

#endif