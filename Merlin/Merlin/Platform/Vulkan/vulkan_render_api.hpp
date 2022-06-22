#ifndef VULKAN_RENDER_API_HPP
#define VULKAN_RENDER_API_HPP
#include "Merlin/Render/render_api.hpp"
#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>


namespace Merlin
{
    const std::vector<const char*> validationLayerNames
    {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


    class VulkanRenderAPI : public RenderAPI
    {
    private:
        GLFWwindow* window;

        VkInstance instance;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice;

        VkDevice logicalDevice;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

    public:
        ~VulkanRenderAPI();
        void Init(void* windowPointer) override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) override;
    private:
        void CreateInstance();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();

    };
}
#endif