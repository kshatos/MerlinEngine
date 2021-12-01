#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <memory>
#include <glm/glm.hpp>


namespace Merlin
{
    class RenderAPI;
    class VertexArray;
    class Shader;

    class Renderer
    {
        static std::unique_ptr<RenderAPI> render_impl;
    public:
        static void Init();
        static void BeginScene();
        static void EndScene();

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();
        static void Submit(
            const std::shared_ptr<Shader>& shader,
            const std::shared_ptr<VertexArray>& vertex_array);
    };
}

#endif