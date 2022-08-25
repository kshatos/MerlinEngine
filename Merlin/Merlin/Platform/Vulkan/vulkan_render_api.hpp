#ifndef VULKAN_RENDER_API_HPP
#define VULKAN_RENDER_API_HPP
// clang-format off
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include "Merlin/Render/render_api.hpp"
#include "imgui.h"

namespace Merlin
{
    const std::vector<const char*> validation_layer_names{
        "VK_LAYER_KHRONOS_validation"};

    const std::vector<const char*> device_extensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    const int MAX_FRAMES_IN_FLIGHT = 2;

    class VulkanRenderAPI : public RenderAPI
    {
    public:
        GLFWwindow* m_window;
        ImGuiContext* m_context;

        VkInstance m_instance;
        VkSurfaceKHR m_surface;

        VkPhysicalDevice m_physical_device;
        VulkanPhysicalDeviceInfo m_physical_device_info;

        VkDevice m_logical_device;
        QueueFamilyIndices m_queue_indices;
        VkQueue m_graphics_queue;
        VkQueue m_present_queue;

        VkCommandPool m_command_pool;
        std::vector<VkCommandBuffer> m_command_buffers;

        VkDescriptorPool m_gui_descriptor_pool;

        VkRenderPass m_im_gui_render_pass;

        VkSwapchainKHR m_swap_chain;
        VkFormat m_swap_chain_image_format;
        VkExtent2D m_swap_chain_extent;
        std::vector<VkImage> m_swap_chain_images;
        std::vector<VkImageView> m_swap_chain_image_views;
        std::vector<VkFramebuffer> m_im_gui_framebuffers;

        std::vector<VkSemaphore> m_image_available_semaphores;
        std::vector<VkSemaphore> m_render_finished_semaphores;
        std::vector<VkFence> m_in_flight_fences;

        uint32_t m_current_frame = 0;
        bool m_framebuffer_resized = false;

    public:
        VulkanRenderAPI() = default;
        ~VulkanRenderAPI();
        // Render API
        void Init(void* window_pointer) override;
        void Shutdown() override;

        void BeginFrame() override;
        void EndFrame() override;
        void PresentFrame() override;

        void RenderScene(const SceneRenderData& scene) override;

        RenderBackend Backend() override;

        std::shared_ptr<MeshBuffer> CreateMeshBuffer(
            float* vertices,
            size_t vertex_count,
            uint32_t* indices,
            size_t index_count,
            BufferLayout vertex_layout) override;
        std::shared_ptr<Material> CreateMaterial(MaterialInfo info) override;
        std::shared_ptr<MaterialInstance> CreateMaterialInstance(
            std::shared_ptr<Material> material) override;
        std::shared_ptr<Shader> CreateShader(
            const std::string& vertex_source,
            const std::string& fragment_source) override;
        std::shared_ptr<Texture2D> CreateTexture2D(
            const Texture2DData& texture_data,
            Texture2DProperties props = Texture2DProperties()) override;
        std::shared_ptr<Cubemap> CreateCubemap(
            const std::vector<std::string>& face_paths) override;
        std::shared_ptr<Cubemap> CreateCubemap(uint32_t resolution,
                                               uint32_t channel_count) override;
        std::shared_ptr<FrameBuffer> CreateFramebuffer(
            const FrameBufferParameters& state) override;

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
        void RecordCommandBuffer(VkCommandBuffer command_buffer,
                                 uint32_t image_index);
    };
}  // namespace Merlin
#endif