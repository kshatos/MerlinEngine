#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <memory>
#include <glm/glm.hpp>
#include "Merlin/Render/camera.hpp"


namespace Merlin
{
    class RenderAPI;
    class VertexArray;
    class Shader;

    class Renderer
    {
        struct SceneData
        {
            glm::mat4 view_matrix;
            glm::mat4 projection_matrix;
        };

        static std::unique_ptr <SceneData> scene_data;
        static std::unique_ptr<RenderAPI> render_impl;
    public:
        static void Init();
        static void BeginScene(const std::shared_ptr<Camera>& camera);
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