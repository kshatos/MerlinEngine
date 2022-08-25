#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Merlin/Render/render_api.hpp"
#include "Merlin/Render/scene_render_data.hpp"

namespace Merlin
{
    class Renderer
    {
        static std::shared_ptr<RenderAPI> m_render_impl;

    public:
        static void Init(std::shared_ptr<RenderAPI> renderApi);
        static void RenderScene(const SceneRenderData& scene);

        static std::shared_ptr<MeshBuffer> CreateMeshBuffer(
            float* vertices,
            size_t vertex_count,
            uint32_t* indices,
            size_t index_count,
            BufferLayout vertexLayout);
        static std::shared_ptr<Material> CreateMaterial(MaterialInfo info);
        static std::shared_ptr<MaterialInstance> CreateMaterialInstance(
            std::shared_ptr<Material> material);
        static std::shared_ptr<Shader> CreateShader(
            const std::string& vertex_source,
            const std::string& fragment_source);
        static std::shared_ptr<Texture2D> CreateTexture2D(
            const Texture2DData& filepath,
            Texture2DProperties props = Texture2DProperties());
        static std::shared_ptr<Cubemap> CreateCubemap(
            const std::vector<std::string>& face_paths);
        static std::shared_ptr<Cubemap> CreateCubemap(uint32_t resolution,
                                                      uint32_t channel_count);
        static std::shared_ptr<FrameBuffer> CreateFramebuffer(
            const FrameBufferParameters& state);
    };
}  // namespace Merlin

#endif