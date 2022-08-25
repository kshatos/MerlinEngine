#include "Merlin/Platform/Vulkan/vulkan_render_api.hpp"

#include <backends/imgui_impl_vulkan.h>

#include <set>
#include <stdexcept>

#include "Merlin/Platform/Vulkan/vulkan_index_buffer.hpp"
#include "Merlin/Platform/Vulkan/vulkan_material.hpp"
#include "Merlin/Platform/Vulkan/vulkan_texture2d.hpp"
#include "Merlin/Platform/Vulkan/vulkan_vertex_buffer.hpp"
#include "backends/imgui_impl_glfw.h"

namespace Merlin
{
    bool validationLayersSupported(std::vector<const char*> layer_names)
    {
        uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count,
                                           available_layers.data());

        for (const char* layer_name : layer_names)
        {
            bool layer_found = false;
            for (const auto& layer_props : available_layers)
            {
                if (strcmp(layer_name, layer_props.layerName))
                {
                    layer_found = true;
                    break;
                }
            }
            if (!layer_found) return false;
        }

        return true;
    }

    // RENDER API
    VulkanRenderAPI::~VulkanRenderAPI() {}

    void VulkanRenderAPI::Init(void* window_pointer)
    {
        m_window = (GLFWwindow*)window_pointer;
        CreateInstance();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
        CreateImageViews();
        CreateImGuiRenderPass();
        CreateImGuiFrameBuffers();
        CreateDescriptorPool();
        CreateCommandPool();
        CreateCommandBuffer();
        CreateSyncObjects();
        CreateImGuiContext();
    }

    void VulkanRenderAPI::Shutdown()
    {
        vkDeviceWaitIdle(m_logical_device);

        for (auto semaphore : m_image_available_semaphores)
            vkDestroySemaphore(m_logical_device, semaphore, nullptr);
        for (auto semaphore : m_render_finished_semaphores)
            vkDestroySemaphore(m_logical_device, semaphore, nullptr);
        for (auto fence : m_in_flight_fences)
            vkDestroyFence(m_logical_device, fence, nullptr);

        CleanupSwapChain();

        vkDestroyDescriptorPool(
            m_logical_device, m_gui_descriptor_pool, nullptr);
        vkDestroyCommandPool(m_logical_device, m_command_pool, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDevice(m_logical_device, nullptr);
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

    void VulkanRenderAPI::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void VulkanRenderAPI::EndFrame() { ImGui::Render(); }

    void VulkanRenderAPI::PresentFrame()
    {
        vkWaitForFences(m_logical_device,
                        1,
                        &m_in_flight_fences[m_current_frame],
                        VK_TRUE,
                        UINT64_MAX);

        uint32_t image_index;
        auto aquire_image_result =
            vkAcquireNextImageKHR(m_logical_device,
                                  m_swap_chain,
                                  UINT64_MAX,
                                  m_image_available_semaphores[m_current_frame],
                                  VK_NULL_HANDLE,
                                  &image_index);

        if (aquire_image_result == VK_ERROR_OUT_OF_DATE_KHR ||
            aquire_image_result == VK_SUBOPTIMAL_KHR || m_framebuffer_resized)
        {
            m_framebuffer_resized = false;
            RecreateSwapChain();
            return;
        }
        else if (aquire_image_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        vkResetFences(
            m_logical_device, 1, &m_in_flight_fences[m_current_frame]);

        vkResetCommandBuffer(m_command_buffers[m_current_frame], 0);
        RecordCommandBuffer(m_command_buffers[m_current_frame], image_index);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore wait_semaphores[] = {
            m_image_available_semaphores[m_current_frame]};
        VkPipelineStageFlags wait_stages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_command_buffers[m_current_frame];

        VkSemaphore signal_semaphores[] = {
            m_render_finished_semaphores[m_current_frame]};
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        auto que_submit_status =
            vkQueueSubmit(m_graphics_queue,
                          1,
                          &submit_info,
                          m_in_flight_fences[m_current_frame]);
        if (que_submit_status != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;

        VkSwapchainKHR swap_chains[] = {m_swap_chain};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swap_chains;
        present_info.pImageIndices = &image_index;
        present_info.pResults = nullptr;

        vkQueuePresentKHR(m_present_queue, &present_info);

        m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanRenderAPI::RenderScene(const SceneRenderData& scene) {}

    RenderBackend VulkanRenderAPI::Backend() { return RenderBackend::Vulkan; }

    std::shared_ptr<MeshBuffer> VulkanRenderAPI::CreateMeshBuffer(
        float* vertices,
        size_t vertex_count,
        uint32_t* indices,
        size_t index_count,
        BufferLayout vertexLayout)
    {
        auto vertex_buffer =
            std::make_shared<VulkanVertexBuffer>(vertices,
                                                 vertex_count,
                                                 vertexLayout,
                                                 m_logical_device,
                                                 m_physical_device,
                                                 m_graphics_queue,
                                                 m_command_pool);
        auto index_buffer =
            std::make_shared<VulkanIndexBuffer>(indices,
                                                index_count,
                                                m_logical_device,
                                                m_physical_device,
                                                m_graphics_queue,
                                                m_command_pool);
        auto mesh_buffer =
            std::make_shared<MeshBuffer>(index_buffer, vertex_buffer);

        return mesh_buffer;
    }

    std::shared_ptr<Material> VulkanRenderAPI::CreateMaterial(MaterialInfo info)
    {
        return std::make_shared<VulkanMaterial>(info);
    }

    std::shared_ptr<MaterialInstance> VulkanRenderAPI::CreateMaterialInstance(
        std::shared_ptr<Material> material)
    {
        return std::make_shared<VulkanMaterialInstance>(material);
    }

    std::shared_ptr<Shader> VulkanRenderAPI::CreateShader(
        const std::string& vertex_source, const std::string& fragment_source)
    {
        return nullptr;
    }

    std::shared_ptr<Texture2D> VulkanRenderAPI::CreateTexture2D(
        const Texture2DData& texture_data, Texture2DProperties props)
    {
        return std::make_shared<VulkanTexture2D>(m_logical_device,
                                                 m_physical_device,
                                                 m_command_pool,
                                                 m_graphics_queue,
                                                 texture_data,
                                                 props);
    }

    std::shared_ptr<Cubemap> VulkanRenderAPI::CreateCubemap(
        const std::vector<std::string>& face_paths)
    {
        return nullptr;
    }

    std::shared_ptr<Cubemap> VulkanRenderAPI::CreateCubemap(
        uint32_t resolution, uint32_t channel_count)
    {
        return nullptr;
    }

    std::shared_ptr<FrameBuffer> VulkanRenderAPI::CreateFramebuffer(
        const FrameBufferParameters& state)
    {
        return nullptr;
    }

    // SETUP
    void VulkanRenderAPI::CreateInstance()
    {
        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Merlin Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        create_info.enabledExtensionCount = glfwExtensionCount;
        create_info.ppEnabledExtensionNames = glfwExtensions;

        if (validationLayersSupported(validation_layer_names))
        {
            create_info.enabledLayerCount =
                static_cast<uint32_t>(validation_layer_names.size());
            create_info.ppEnabledLayerNames = validation_layer_names.data();
        }
        else
        {
            create_info.enabledLayerCount = 0;
        }

        auto instanceCreateResult =
            vkCreateInstance(&create_info, nullptr, &m_instance);
        if (instanceCreateResult != VK_SUCCESS)
        {
            throw std::runtime_error("Unable to create vulkan instance!");
        }
    }

    void VulkanRenderAPI::CreateSurface()
    {
        auto surface_create_result =
            glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface);
        if (surface_create_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void VulkanRenderAPI::PickPhysicalDevice()
    {
        uint32_t device_count;
        vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

        float best_score = -std::numeric_limits<float>::infinity();
        VkPhysicalDevice selected_device = VK_NULL_HANDLE;
        for (const auto& device : devices)
        {
            VulkanPhysicalDeviceInfo device_info(device, m_instance, m_surface);
            float score = EvaluateDevice(device_info);
            if (score > best_score)
            {
                best_score = score;
                selected_device = device;
            }
        }

        if (selected_device == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Unable to find acceptable GPU!");
        }

        m_physical_device = selected_device;
        m_physical_device_info =
            VulkanPhysicalDeviceInfo(selected_device, m_instance, m_surface);
    }

    void VulkanRenderAPI::CreateLogicalDevice()
    {
        m_queue_indices = FindQueueFamilies(m_physical_device, m_surface);
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t> unique_queue_families = {
            m_queue_indices.graphics_family.value(),
            m_queue_indices.present_family.value()};

        float queue_priority = 1.0f;
        for (auto& queue_family_index : unique_queue_families)
        {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType =
                VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family_index;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        VkPhysicalDeviceFeatures device_features{};
        device_features.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = queue_create_infos.size();
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.pEnabledFeatures = &device_features;

        create_info.enabledExtensionCount =
            static_cast<uint32_t>(device_extensions.size());
        create_info.ppEnabledExtensionNames = device_extensions.data();

        if (validation_layer_names.size() > 0)
        {
            create_info.enabledLayerCount =
                static_cast<uint32_t>(validation_layer_names.size());
            create_info.ppEnabledLayerNames = validation_layer_names.data();
        }
        else
        {
            create_info.enabledLayerCount = 0;
        }

        auto create_result = vkCreateDevice(
            m_physical_device, &create_info, nullptr, &m_logical_device);
        if (create_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(m_logical_device,
                         m_queue_indices.graphics_family.value(),
                         0,
                         &m_graphics_queue);
        vkGetDeviceQueue(m_logical_device,
                         m_queue_indices.present_family.value(),
                         0,
                         &m_present_queue);
    }

    void VulkanRenderAPI::CreateSwapChain()
    {
        SwapChainSupportDetails swap_chain_support =
            QuerySwapChainSupport(m_physical_device, m_surface);

        VkSurfaceFormatKHR surface_format =
            ChooseSwapSurfaceFormat(swap_chain_support.formats);
        VkPresentModeKHR presentMode =
            ChooseSwapPresentMode(swap_chain_support.present_modes);
        VkExtent2D extent =
            ChooseSwapExtent(swap_chain_support.capabilities, m_window);

        uint32_t image_count =
            swap_chain_support.capabilities.minImageCount + 1;
        if (swap_chain_support.capabilities.maxImageCount > 0 &&
            image_count > swap_chain_support.capabilities.maxImageCount)
        {
            image_count = swap_chain_support.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = m_surface;
        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices =
            FindQueueFamilies(m_physical_device, m_surface);
        uint32_t queue_family_indices[] = {indices.graphics_family.value(),
                                           indices.present_family.value()};
        if (indices.graphics_family != indices.present_family)
        {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        }
        else
        {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0;
            create_info.pQueueFamilyIndices = nullptr;
        }
        create_info.preTransform =
            swap_chain_support.capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = presentMode;
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = VK_NULL_HANDLE;

        VkResult swapchain_create_result = vkCreateSwapchainKHR(
            m_logical_device, &create_info, nullptr, &m_swap_chain);

        if (swapchain_create_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(
            m_logical_device, m_swap_chain, &image_count, nullptr);
        m_swap_chain_images.resize(image_count);
        vkGetSwapchainImagesKHR(m_logical_device,
                                m_swap_chain,
                                &image_count,
                                m_swap_chain_images.data());
        m_swap_chain_image_format = surface_format.format;
        m_swap_chain_extent = extent;
    }

    void VulkanRenderAPI::CreateImageViews()
    {
        m_swap_chain_image_views.resize(m_swap_chain_images.size());
        for (size_t i = 0; i < m_swap_chain_images.size(); i++)
        {
            VkImageViewCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = m_swap_chain_images[i];
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = m_swap_chain_image_format;
            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;

            auto result = vkCreateImageView(m_logical_device,
                                            &create_info,
                                            nullptr,
                                            &m_swap_chain_image_views[i]);
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }

    void VulkanRenderAPI::CreateCommandPool()
    {
        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = m_queue_indices.graphics_family.value();

        auto pool_create_status = vkCreateCommandPool(
            m_logical_device, &pool_info, nullptr, &m_command_pool);
        if (pool_create_status != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void VulkanRenderAPI::CreateCommandBuffer()
    {
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = m_command_pool;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = (uint32_t)MAX_FRAMES_IN_FLIGHT;

        m_command_buffers.resize(MAX_FRAMES_IN_FLIGHT);
        auto buffer_create_status = vkAllocateCommandBuffers(
            m_logical_device, &alloc_info, m_command_buffers.data());
        if (buffer_create_status != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void VulkanRenderAPI::CreateDescriptorPool()
    {
        std::vector<VkDescriptorPoolSize> pool_sizes = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * (uint32_t)pool_sizes.size();
        pool_info.poolSizeCount = (uint32_t)pool_sizes.size();
        pool_info.pPoolSizes = pool_sizes.data();

        auto create_result = vkCreateDescriptorPool(
            m_logical_device, &pool_info, nullptr, &m_gui_descriptor_pool);
        if (create_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void VulkanRenderAPI::CreateSyncObjects()
    {
        m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            auto image_semaphore_result =
                vkCreateSemaphore(m_logical_device,
                                  &semaphore_info,
                                  nullptr,
                                  &m_image_available_semaphores[i]);
            auto render_semaphore_result =
                vkCreateSemaphore(m_logical_device,
                                  &semaphore_info,
                                  nullptr,
                                  &m_render_finished_semaphores[i]);
            auto fence_result = vkCreateFence(
                m_logical_device, &fence_info, nullptr, &m_in_flight_fences[i]);

            if (image_semaphore_result != VK_SUCCESS ||
                render_semaphore_result != VK_SUCCESS ||
                fence_result != VK_SUCCESS)
            {
                throw std::runtime_error("Unable to create sync objects!");
            }
        }
    }

    void VulkanRenderAPI::CreateImGuiRenderPass()
    {
        VkAttachmentDescription attachment = {};
        attachment.format = m_swap_chain_image_format;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        // TODO: switch to  VK_ATTACHMENT_LOAD_OP_LOAD when other passes added
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment = {};
        color_attachment.attachment = 0;
        color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask =
            0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;
        info.dependencyCount = 1;
        info.pDependencies = &dependency;

        auto create_status = vkCreateRenderPass(
            m_logical_device, &info, nullptr, &m_im_gui_render_pass);
        if (create_status != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Could not create Dear ImGui's render pass");
        }
    }

    void VulkanRenderAPI::CreateImGuiFrameBuffers()
    {
        m_im_gui_framebuffers.resize(m_swap_chain_image_views.size());
        for (size_t i = 0; i < m_swap_chain_image_views.size(); i++)
        {
            VkImageView attachments[] = {m_swap_chain_image_views[i]};
            VkFramebufferCreateInfo framebuffer_info{};
            framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_info.renderPass = m_im_gui_render_pass;
            framebuffer_info.attachmentCount = 1;
            framebuffer_info.pAttachments = attachments;
            framebuffer_info.width = m_swap_chain_extent.width;
            framebuffer_info.height = m_swap_chain_extent.height;
            framebuffer_info.layers = 1;

            auto framebuffer_create_status =
                vkCreateFramebuffer(m_logical_device,
                                    &framebuffer_info,
                                    nullptr,
                                    &m_im_gui_framebuffers[i]);
            if (framebuffer_create_status != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void VulkanRenderAPI::CreateImGuiContext()
    {
        IMGUI_CHECKVERSION();
        m_context = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_instance;
        init_info.PhysicalDevice = m_physical_device;
        init_info.Device = m_logical_device;

        init_info.QueueFamily = m_queue_indices.graphics_family.value();
        init_info.Queue = m_graphics_queue;
        init_info.PipelineCache = VK_NULL_HANDLE;  // TODO: Figure out if needed
        init_info.DescriptorPool = m_gui_descriptor_pool;
        init_info.Allocator = nullptr;  // TODO: Figure out if needed
        init_info.MinImageCount = m_swap_chain_images.size();
        init_info.ImageCount = m_swap_chain_images.size();

        init_info.CheckVkResultFn = [](VkResult err)
        {
            if (err != VK_SUCCESS)
                throw std::runtime_error(
                    "Error initializing imgui vulkan backend!");
        };
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)m_window, true);
        ImGui_ImplVulkan_Init(&init_info, m_im_gui_render_pass);

        auto command_buffer =
            BeginSingleTimeCommands(m_logical_device, m_command_pool);
        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
        EndSingleTimeCommands(
            m_logical_device, command_buffer, m_command_pool, m_graphics_queue);
    }

    void VulkanRenderAPI::RecreateSwapChain()
    {
        int width = 0;
        int height = 0;
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(m_window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_logical_device);

        CleanupSwapChain();

        CreateSwapChain();
        CreateImageViews();
        CreateImGuiRenderPass();
        CreateImGuiFrameBuffers();
    }

    void VulkanRenderAPI::CleanupSwapChain()
    {
        for (auto framebuffer : m_im_gui_framebuffers)
            vkDestroyFramebuffer(m_logical_device, framebuffer, nullptr);

        vkDestroyRenderPass(m_logical_device, m_im_gui_render_pass, nullptr);

        for (auto image_view : m_swap_chain_image_views)
            vkDestroyImageView(m_logical_device, image_view, nullptr);

        vkDestroySwapchainKHR(m_logical_device, m_swap_chain, nullptr);
    }

    void VulkanRenderAPI::RecordCommandBuffer(VkCommandBuffer command_buffer,
                                              uint32_t image_index)
    {
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = 0;
        begin_info.pInheritanceInfo = nullptr;

        auto begin_status = vkBeginCommandBuffer(command_buffer, &begin_info);
        if (begin_status != VK_SUCCESS)
        {
            throw std::runtime_error(
                "failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_im_gui_render_pass;
        render_pass_info.framebuffer = m_im_gui_framebuffers[image_index];
        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = m_swap_chain_extent;
        VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(
            command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        {
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                            command_buffer);
        }
        vkCmdEndRenderPass(command_buffer);

        if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

}  // namespace Merlin