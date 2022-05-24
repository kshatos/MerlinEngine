#ifndef VULKAN_RENDER_API_HPP
#define VULKAN_RENDER_API_HPP
#include "Merlin/Render/render_api.hpp"
#include "vulkan/vulkan.h"


namespace Merlin
{
    class VulkanRenderAPI : public RenderAPI
    {
        void Init() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) override;
    private:
        VkInstance instance;
    };
}
#endif