#ifndef VULKAN_RENDER_API_HPP
#define VULKAN_RENDER_API_HPP
#include "Merlin/Render/render_api.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>
#include "imgui.h"


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
        ImGuiContext* context;

        VkInstance instance;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice;
        VulkanPhysicalDeviceInfo physicalDeviceInfo;

        VkDevice logicalDevice;
        QueueFamilyIndices queueIndices;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        VkDescriptorPool guiDescriptorPool;

        VkRenderPass imGuiRenderPass;

        VkSwapchainKHR swapChain;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> imGuiFramebuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        uint32_t currentFrame = 0;
        bool framebufferResized = false;

    public:
        VulkanRenderAPI() = default;
        ~VulkanRenderAPI();
        // Render API
        void Init(void* windowPointer) override;
        void Shutdown() override;

        void BeginFrame() override;
        void EndFrame() override;
        void PresentFrame() override;

        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawTriangles(const std::shared_ptr<VertexArray>& vertex_array) override;
        void RenderScene(const SceneRenderData& scene) override;

        RenderBackend Backend() override;

        std::shared_ptr<VertexBuffer> CreateVertexBuffer(
            float* vertices, size_t size) override;
        std::shared_ptr<IndexBuffer> CreateIndexBuffer(
            uint32_t* indices, uint32_t index_count) override;
        std::shared_ptr<VertexArray> CreateVertexArray() override;
        std::shared_ptr<Shader> CreateShader(
            const std::string& vertex_source,
            const std::string& fragment_source) override;
        std::shared_ptr<Texture2D> CreateTexture2D(
            const std::string& filepath,
            Texture2DProperties props = Texture2DProperties()) override;
        std::shared_ptr<Texture2D> CreateTexture2D(
            void* data,
            uint32_t width,
            uint32_t height,
            uint32_t channel_count,
            Texture2DProperties props = Texture2DProperties()) override;
        std::shared_ptr<Cubemap> CreateCubemap(
            const std::vector<std::string>& face_paths) override;
        std::shared_ptr<Cubemap> CreateCubemap(
            uint32_t resolution, uint32_t channel_count) override;
        std::shared_ptr<FrameBuffer> CreateFramebuffer(
            const FrameBufferParameters& state) override;

        // Vulkan Specific
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    private:
        // Setup stages
        void CreateInstance();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateImGuiContext();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateCommandPool();
        void CreateCommandBuffer();
        void CreateDescriptorPool();
        void CreateImGuiRenderPass();
        void CreateImGuiFrameBuffers();
        void CreateSyncObjects();

        void RecreateSwapChain();
        void CleanupSwapChain();

        // Render
        void RecordCommandBuffer(
            VkCommandBuffer commandBuffer,
            uint32_t imageIndex);
    };
}
#endif