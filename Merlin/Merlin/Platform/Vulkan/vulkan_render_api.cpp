#include "Merlin/Platform/Vulkan/vulkan_render_api.hpp"
#include "Merlin/Platform/Vulkan/vulkan_vertex_buffer.hpp"
#include "Merlin/Platform/Vulkan/vulkan_index_buffer.hpp"
#include "Merlin/Platform/Vulkan/vulkan_texture2d.hpp"
#include "Merlin/Platform/Vulkan/vulkan_material.hpp"
#include <stdexcept>
#include <set>
#include <backends/imgui_impl_vulkan.h>
#include "backends/imgui_impl_glfw.h"



namespace Merlin
{
    bool validationLayersSupported(
        std::vector<const char*> layerNames)
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : layerNames)
        {
            bool layerFound = false;
            for (const auto& layerProps : availableLayers)
            {
                if (strcmp(layerName, layerProps.layerName))
                {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound)
                return false;
        }

        return true;
    }

    // RENDER API
    VulkanRenderAPI::~VulkanRenderAPI()
    {
    }

    void VulkanRenderAPI::Init(void* windowPointer)
    {
        window = (GLFWwindow*)windowPointer;
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
        vkDeviceWaitIdle(logicalDevice);

        for (auto semaphore : imageAvailableSemaphores)
            vkDestroySemaphore(logicalDevice, semaphore, nullptr);
        for (auto semaphore : renderFinishedSemaphores)
            vkDestroySemaphore(logicalDevice, semaphore, nullptr);
        for (auto fence : inFlightFences)
            vkDestroyFence(logicalDevice, fence, nullptr);

        CleanupSwapChain();

        vkDestroyDescriptorPool(logicalDevice, guiDescriptorPool, nullptr);
        vkDestroyCommandPool(logicalDevice, commandPool, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDevice(logicalDevice, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }


    void VulkanRenderAPI::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void VulkanRenderAPI::EndFrame()
    {
        ImGui::Render();
    }

    void VulkanRenderAPI::PresentFrame()
    {
        vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        auto aquireImageResult = vkAcquireNextImageKHR(
            logicalDevice, swapChain,
            UINT64_MAX, imageAvailableSemaphores[currentFrame],
            VK_NULL_HANDLE, &imageIndex);

        if (aquireImageResult == VK_ERROR_OUT_OF_DATE_KHR ||
            aquireImageResult == VK_SUBOPTIMAL_KHR ||
            framebufferResized)
        {
            framebufferResized = false;
            RecreateSwapChain();
            return;
        }
        else if (aquireImageResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);

        vkResetCommandBuffer(commandBuffers[currentFrame], 0);
        RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);


        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        auto queSubmitStatus = vkQueueSubmit(
            graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
        if (queSubmitStatus != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        vkQueuePresentKHR(presentQueue, &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanRenderAPI::RenderScene(const SceneRenderData& scene)
    {
    }

    RenderBackend VulkanRenderAPI::Backend()
    {
        return RenderBackend::Vulkan;
    }

    std::shared_ptr<MeshBuffer> VulkanRenderAPI::CreateMeshBuffer(
        float* vertices,
        size_t vertex_count,
        uint32_t* indices,
        size_t index_count,
        BufferLayout vertexLayout)
    {
        auto vertexBuffer = std::make_shared<VulkanVertexBuffer>(
            vertices,
            vertex_count,
            vertexLayout,
            logicalDevice,
            physicalDevice,
            graphicsQueue,
            commandPool);
        auto indexBuffer = std::make_shared<VulkanIndexBuffer>(
            indices,
            index_count,
            logicalDevice,
            physicalDevice,
            graphicsQueue,
            commandPool);
        auto meshBuffer = std::make_shared<MeshBuffer>(
            indexBuffer, vertexBuffer);

        return meshBuffer;
    }

    std::shared_ptr<Material> VulkanRenderAPI::CreateMaterial(
        MaterialInfo info)
    {
        return std::make_shared<VulkanMaterial>(info);
    }

    std::shared_ptr<MaterialInstance> VulkanRenderAPI::CreateMaterialInstance(
        std::shared_ptr<Material> material)
    {
        return std::make_shared<VulkanMaterialInstance>(material);
    }

    std::shared_ptr<Shader> VulkanRenderAPI::CreateShader(
        const std::string& vertex_source,
        const std::string& fragment_source)
    {
        return nullptr;
    }

    std::shared_ptr<Texture2D> VulkanRenderAPI::CreateTexture2D(
        const Texture2DData& texture_data,
        Texture2DProperties props)
    {
        return std::make_shared<VulkanTexture2D>(
            logicalDevice,
            physicalDevice,
            commandPool,
            graphicsQueue,
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
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Merlin Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        if (validationLayersSupported(validationLayerNames))
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayerNames.size());
            createInfo.ppEnabledLayerNames = validationLayerNames.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        auto instanceCreateResult = vkCreateInstance(
            &createInfo, nullptr, &instance);
        if (instanceCreateResult != VK_SUCCESS)
        {
            throw std::runtime_error("Unable to create vulkan instance!");
        }
    }

    void VulkanRenderAPI::CreateSurface()
    {
        auto surfaceCreateResult = glfwCreateWindowSurface(
            instance, window, nullptr, &surface);
        if (surfaceCreateResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void VulkanRenderAPI::PickPhysicalDevice()
    {
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(
            instance, &deviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(
            instance, &deviceCount, devices.data());

        float bestScore = -std::numeric_limits<float>::infinity();
        VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;
        for (const auto& device : devices)
        {
            VulkanPhysicalDeviceInfo deviceInfo(device, instance, surface);
            float score = evaluateDevice(deviceInfo);
            if (score > bestScore)
            {
                bestScore = score;
                selectedDevice = device;
            }
        }

        if (selectedDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Unable to find acceptable GPU!");
        }

        physicalDevice = selectedDevice;
        physicalDeviceInfo = VulkanPhysicalDeviceInfo(
            selectedDevice, instance, surface);
    }

    void VulkanRenderAPI::CreateLogicalDevice()
    {
        queueIndices = findQueueFamilies(physicalDevice, surface);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies =
        {
            queueIndices.graphicsFamily.value(),
            queueIndices.presentFamily.value()
        };

        float queuePriority = 1.0f;
        for (auto& queueFamilyIndex : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }
        
        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = queueCreateInfos.size();
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;


        createInfo.enabledExtensionCount =
            static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (validationLayerNames.size() > 0)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayerNames.size());
            createInfo.ppEnabledLayerNames = validationLayerNames.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        auto createResult = vkCreateDevice(
            physicalDevice, &createInfo, nullptr, &logicalDevice);
        if (createResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(logicalDevice, queueIndices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(logicalDevice, queueIndices.presentFamily.value(), 0, &presentQueue);
    }

    void VulkanRenderAPI::CreateSwapChain()
    {
        SwapChainSupportDetails swapChainSupport =
            querySwapChainSupport(physicalDevice, surface);

        VkSurfaceFormatKHR surfaceFormat =
            chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode =
            chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent =
            chooseSwapExtent(swapChainSupport.capabilities, window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
        uint32_t queueFamilyIndices[] = {
            indices.graphicsFamily.value(),
            indices.presentFamily.value()
        };
        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult swapchainCreateResult = vkCreateSwapchainKHR(
            logicalDevice, &createInfo, nullptr, &swapChain);

        if (swapchainCreateResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(
            logicalDevice, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(
            logicalDevice, swapChain, &imageCount, swapChainImages.data());
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    void VulkanRenderAPI::CreateImageViews()
    {
        swapChainImageViews.resize(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            auto result = vkCreateImageView(
                logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]);
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }

    void VulkanRenderAPI::CreateCommandPool()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueIndices.graphicsFamily.value();

        auto poolCreateStatus = vkCreateCommandPool(
            logicalDevice, &poolInfo, nullptr, &commandPool);
        if (poolCreateStatus != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void VulkanRenderAPI::CreateCommandBuffer()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)MAX_FRAMES_IN_FLIGHT;

        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        auto bufferCreateStatus = vkAllocateCommandBuffers(
            logicalDevice, &allocInfo, commandBuffers.data());
        if (bufferCreateStatus != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void VulkanRenderAPI::CreateDescriptorPool()
    {
        std::vector<VkDescriptorPoolSize> poolSizes =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * (uint32_t)poolSizes.size();
        poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();

        auto createResult = vkCreateDescriptorPool(
            logicalDevice, &poolInfo, nullptr, &guiDescriptorPool);
        if (createResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void VulkanRenderAPI::CreateSyncObjects()
    {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            auto imageSemaphoreResult = vkCreateSemaphore(
                logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
            auto renderSemaphoreResult = vkCreateSemaphore(
                logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
            auto fenceResult = vkCreateFence(
                logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]);

            if (imageSemaphoreResult != VK_SUCCESS ||
                renderSemaphoreResult != VK_SUCCESS ||
                fenceResult != VK_SUCCESS)
            {
                throw std::runtime_error("Unable to create sync objects!");
            }
        }
    }

    void VulkanRenderAPI::CreateImGuiRenderPass()
    {
        VkAttachmentDescription attachment = {};
        attachment.format = swapChainImageFormat;
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
        dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;
        info.dependencyCount = 1;
        info.pDependencies = &dependency;

        auto createStatus = vkCreateRenderPass(
            logicalDevice, &info, nullptr, &imGuiRenderPass);
        if (createStatus != VK_SUCCESS)
        {
            throw std::runtime_error("Could not create Dear ImGui's render pass");
        }
    }

    void VulkanRenderAPI::CreateImGuiFrameBuffers()
    {
        imGuiFramebuffers.resize(swapChainImageViews.size());
        for (size_t i = 0; i < swapChainImageViews.size(); i++)
        {
            VkImageView attachments[] =
            {
                swapChainImageViews[i]
            };
            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType =
                VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = imGuiRenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            auto framebufferCreateStatus = vkCreateFramebuffer(
                logicalDevice, &framebufferInfo, nullptr, &imGuiFramebuffers[i]);
            if (framebufferCreateStatus != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void VulkanRenderAPI::CreateImGuiContext()
    {
        IMGUI_CHECKVERSION();
        context = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance;
        init_info.PhysicalDevice = physicalDevice;
        init_info.Device = logicalDevice;

        init_info.QueueFamily = queueIndices.graphicsFamily.value();
        init_info.Queue = graphicsQueue;
        init_info.PipelineCache = VK_NULL_HANDLE; // TODO: Figure out if needed
        init_info.DescriptorPool = guiDescriptorPool;
        init_info.Allocator = nullptr; // TODO: Figure out if needed
        init_info.MinImageCount = swapChainImages.size();
        init_info.ImageCount = swapChainImages.size();

        init_info.CheckVkResultFn = [](VkResult err)
        {
            if (err != VK_SUCCESS)
                throw std::runtime_error("Error initializing imgui vulkan backend!");
        };
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window, true);
        ImGui_ImplVulkan_Init(&init_info, imGuiRenderPass);

        auto commandBuffer = BeginSingleTimeCommands(logicalDevice, commandPool);
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        EndSingleTimeCommands(logicalDevice, commandBuffer, commandPool, graphicsQueue);
    }

    void VulkanRenderAPI::RecreateSwapChain()
    {
        int width = 0;
        int height = 0;
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(logicalDevice);

        CleanupSwapChain();

        CreateSwapChain();
        CreateImageViews();
        CreateImGuiRenderPass();
        CreateImGuiFrameBuffers();
    }

    void VulkanRenderAPI::CleanupSwapChain()
    {
        for (auto framebuffer : imGuiFramebuffers)
            vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);

        vkDestroyRenderPass(logicalDevice, imGuiRenderPass, nullptr);

        for (auto imageView : swapChainImageViews)
            vkDestroyImageView(logicalDevice, imageView, nullptr);

        vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
    }

    void VulkanRenderAPI::RecordCommandBuffer(
        VkCommandBuffer commandBuffer,
        uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        auto beginStatus = vkBeginCommandBuffer(commandBuffer, &beginInfo);
        if (beginStatus != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = imGuiRenderPass;
        renderPassInfo.framebuffer = imGuiFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChainExtent;
        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        {
            ImGui_ImplVulkan_RenderDrawData(
                ImGui::GetDrawData(),
                commandBuffer);
        }
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

}