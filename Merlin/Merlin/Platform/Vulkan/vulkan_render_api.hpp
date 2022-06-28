#ifndef VULKAN_RENDER_API_HPP
#define VULKAN_RENDER_API_HPP
#include "Merlin/Render/render_api.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
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

    const int MAX_FRAMES_IN_FLIGHT = 2;


    class VulkanRenderAPI : public RenderAPI
    {
    public:
        GLFWwindow* window;

        VkInstance instance;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice;
        VulkanPhysicalDeviceInfo physicalDeviceInfo;

        VkDevice logicalDevice;
        QueueFamilyIndices queueIndices;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        VkDescriptorPool guiDescriptorPool;

        VkRenderPass imGuiRenderPass;

        VkSwapchainKHR swapChain;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

    public:
        VulkanRenderAPI() = default;
        ~VulkanRenderAPI();
        // Render API
        void Init(void* windowPointer) override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) override;
        void SwapBuffers() override;
        RenderBackend Backend() override;

        // Vulkan Specific
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    private:
        // Setup stages
        void CreateInstance();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateCommandPool();
        void CreateCommandBuffer();
        void CreateDescriptorPool();
        void CreateImGuiRenderPass();
        void CreateSyncObjects();
    };
}
#endif