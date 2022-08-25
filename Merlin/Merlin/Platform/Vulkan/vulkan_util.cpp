#include "Merlin/Platform/Vulkan/vulkan_util.hpp"

#include <algorithm>
#include <set>
#include <stdexcept>

#include "Merlin/Core/logger.hpp"

namespace Merlin
{
    //////////////////////////////
    // PHYSICAL DEVICE
    //////////////////////////////
    VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(
        VkPhysicalDevice device_handle,
        VkInstance instance,
        VkSurfaceKHR surface)
        : surface_capabilities()
    {
        handle = device_handle;

        // Base details
        vkGetPhysicalDeviceProperties(device_handle, &device_properties);
        vkGetPhysicalDeviceFeatures(device_handle, &device_features);

        // Extension support
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(
            device_handle, nullptr, &extension_count, nullptr);

        extension_properties.resize(extension_count);
        vkEnumerateDeviceExtensionProperties(device_handle,
                                             nullptr,
                                             &extension_count,
                                             extension_properties.data());

        // Queue families
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            device_handle, &queue_family_count, nullptr);
        queue_family_properties.resize(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(
            device_handle, &queue_family_count, queue_family_properties.data());

        // Surface support
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            device_handle, surface, &surface_capabilities);

        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device_handle, surface, &format_count, nullptr);

        if (format_count != 0)
        {
            surface_formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                device_handle, surface, &format_count, surface_formats.data());
        }

        uint32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device_handle, surface, &present_mode_count, nullptr);
        if (present_mode_count > 0)
        {
            surface_present_modes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device_handle,
                surface,
                &present_mode_count,
                surface_present_modes.data());
        }
    }

    float EvaluateDevice(const VulkanPhysicalDeviceInfo& info) { return 1.0f; }

    //////////////////////////////
    // LOGICAL DEVICE
    //////////////////////////////
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device,
                                         VkSurfaceKHR surface)
    {
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            device, &queue_family_count, nullptr);
        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(
            device, &queue_family_count, queue_families.data());

        QueueFamilyIndices indices;
        int i = 0;
        for (const auto& family_props : queue_families)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(
                device, i, surface, &presentSupport);
            if (presentSupport)
            {
                indices.present_family = i;
            }

            if (family_props.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphics_family = i;
            i++;
        }

        return indices;
    }

    //////////////////////////////
    // SWAP CHAIN
    //////////////////////////////
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device,
                                                  VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            device, surface, &details.capabilities);

        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, surface, &format_count, nullptr);

        if (format_count != 0)
        {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                device, surface, &format_count, details.formats.data());
        }

        uint32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &present_mode_count, nullptr);
        if (present_mode_count > 0)
        {
            details.present_modes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                surface,
                &present_mode_count,
                details.present_modes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& available_formats)
    {
        // Check for prefered
        for (const auto& available_format : available_formats)
        {
            if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                available_format.colorSpace ==
                    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return available_format;
            }
        }

        // Use whatevers available
        return available_formats[0];
    }

    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& available_present_modes)
    {
        // Check for prefered
        for (const auto& avialabe_mode : available_present_modes)
        {
            if (avialabe_mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return avialabe_mode;
            }
        }

        // Guaranteed otherwise
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                                GLFWwindow* window)
    {
        if (capabilities.currentExtent.width !=
            std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actual_extent = {static_cast<uint32_t>(width),
                                    static_cast<uint32_t>(height)};
        actual_extent.width = std::clamp(actual_extent.width,
                                         capabilities.minImageExtent.width,
                                         capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height,

                                          capabilities.minImageExtent.height,
                                          capabilities.maxImageExtent.height);

        return actual_extent;
    }

    VkCommandBuffer BeginSingleTimeCommands(VkDevice logicalDevice,
                                            VkCommandPool commandPool)
    {
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandPool = commandPool;
        alloc_info.commandBufferCount = 1;

        VkCommandBuffer command_buffer;
        vkAllocateCommandBuffers(logicalDevice, &alloc_info, &command_buffer);

        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(command_buffer, &begin_info);

        return command_buffer;
    }

    void EndSingleTimeCommands(VkDevice logical_device,
                               VkCommandBuffer command_buffer,
                               VkCommandPool command_pool,
                               VkQueue queue)
    {
        vkEndCommandBuffer(command_buffer);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;

        vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(logical_device, command_pool, 1, &command_buffer);
    }

    //////////////////////////////
    // BUFFERS
    //////////////////////////////
    uint32_t FindMemoryType(uint32_t type_filter,
                            VkMemoryPropertyFlags properties,
                            VkPhysicalDevice physical_device)
    {
        VkPhysicalDeviceMemoryProperties mem_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);
        for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
        {
            if (type_filter & (1 << i) &&
                (mem_properties.memoryTypes[i].propertyFlags & properties) ==
                    properties)
            {
                return i;
            }
        }
        throw std::runtime_error("failed to find suitable memory type!");

        return 0;
    }

    void CreateBuffer(VkDevice device,
                      VkPhysicalDevice physicalDevice,
                      VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo buffer_info{};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = size;
        buffer_info.usage = usage;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &buffer_info, nullptr, &buffer) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements mem_requirements;
        vkGetBufferMemoryRequirements(device, buffer, &mem_requirements);

        VkMemoryAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = mem_requirements.size;
        alloc_info.memoryTypeIndex = FindMemoryType(
            mem_requirements.memoryTypeBits, properties, physicalDevice);

        if (vkAllocateMemory(device, &alloc_info, nullptr, &bufferMemory) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    void CopyBuffer(VkQueue queue,
                    VkDevice device,
                    VkCommandPool command_pool,
                    VkBuffer src_buffer,
                    VkBuffer dst_buffer,
                    VkDeviceSize size)
    {
        VkCommandBuffer command_buffer =
            BeginSingleTimeCommands(device, command_pool);
        {
            VkBufferCopy copy_region{};
            copy_region.srcOffset = 0;
            copy_region.dstOffset = 0;
            copy_region.size = size;
            vkCmdCopyBuffer(
                command_buffer, src_buffer, dst_buffer, 1, &copy_region);
        }
        EndSingleTimeCommands(device, command_buffer, command_pool, queue);
    }

    //////////////////////////////
    // IMAGES
    //////////////////////////////
    /*
    TODO:
    Find a better way of figuring out available image
    formats instead of hardcoding
    */
    VkFormat GetVkChannelFormat(const uint32_t& channel_count)
    {
        switch (channel_count)
        {
            case 1:
                return VK_FORMAT_R8_SRGB;
            case 2:
                return VK_FORMAT_R8G8_SRGB;
            case 3:
                return VK_FORMAT_R8G8B8_SRGB;
            case 4:
                return VK_FORMAT_R8G8B8A8_SRGB;
            default:
                ME_LOG_ERROR("Invalid number of texture channels.");
                return VK_FORMAT_UNDEFINED;
        }
    }

    VkFilter GetVkFilterMode(TextureFilterMode filter_mode)
    {
        switch (filter_mode)
        {
            case TextureFilterMode::Linear:
                return VK_FILTER_LINEAR;
            case TextureFilterMode::Nearest:
                return VK_FILTER_NEAREST;
            default:
                ME_LOG_ERROR("Invalid filter mode.");
                return VK_FILTER_NEAREST;
        }
    }

    VkSamplerAddressMode GetVkAdressMode(TextureWrapMode wrap_mode)
    {
        switch (wrap_mode)
        {
            case TextureWrapMode::ClampToEdge:
                return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
            case TextureWrapMode::ClampToBorder:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            case TextureWrapMode::MirroredRepeat:
                return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case TextureWrapMode::Repeat:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            default:
                ME_LOG_ERROR("Invalid texture wrap mode.");
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        }
    }

    void CreateImage(VkDevice logical_device,
                     VkPhysicalDevice physical_device,
                     uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage& image,
                     VkDeviceMemory& image_memory)
    {
        VkImageCreateInfo image_info{};
        image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.imageType = VK_IMAGE_TYPE_2D;
        image_info.extent.width = width;
        image_info.extent.height = height;
        image_info.extent.depth = 1;
        image_info.mipLevels = 1;
        image_info.arrayLayers = 1;
        image_info.format = format;
        image_info.tiling = tiling;
        image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.usage = usage;
        image_info.samples = VK_SAMPLE_COUNT_1_BIT;
        image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        auto image_create_result =
            vkCreateImage(logical_device, &image_info, nullptr, &image);
        if (image_create_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements mem_requirements;
        vkGetImageMemoryRequirements(logical_device, image, &mem_requirements);

        VkMemoryAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = mem_requirements.size;
        alloc_info.memoryTypeIndex = FindMemoryType(
            mem_requirements.memoryTypeBits, properties, physical_device);

        auto memory_allocate_result = vkAllocateMemory(
            logical_device, &alloc_info, nullptr, &image_memory);
        if (memory_allocate_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(logical_device, image, image_memory, 0);
    }

    void CopyBufferToImage(VkDevice logical_device,
                           VkCommandPool command_pool,
                           VkQueue queue,
                           VkBuffer buffer,
                           VkImage image,
                           uint32_t width,
                           uint32_t height)
    {
        VkCommandBuffer command_buffer =
            BeginSingleTimeCommands(logical_device, command_pool);
        {
            VkBufferImageCopy region{};
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;

            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;

            region.imageOffset = {0, 0, 0};
            region.imageExtent = {width, height, 1};

            vkCmdCopyBufferToImage(command_buffer,
                                   buffer,
                                   image,
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                   1,
                                   &region);
        }
        EndSingleTimeCommands(
            logical_device, command_buffer, command_pool, queue);
    }

    void TransitionImageLayout(VkDevice logical_device,
                               VkCommandPool command_pool,
                               VkQueue queue,
                               VkImage image,
                               VkFormat format,
                               VkImageLayout old_layout,
                               VkImageLayout new_layout)
    {
        VkCommandBuffer command_buffer =
            BeginSingleTimeCommands(logical_device, command_pool);
        {
            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = old_layout;
            barrier.newLayout = new_layout;

            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            barrier.image = image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            VkPipelineStageFlags source_stage;
            VkPipelineStageFlags destination_stage;
            if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
                new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
            {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            }
            else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                     new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            {
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }
            else
            {
                throw std::invalid_argument("unsupported layout transition!");
            }

            vkCmdPipelineBarrier(command_buffer,
                                 source_stage,
                                 destination_stage,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrier);
        }
        EndSingleTimeCommands(
            logical_device, command_buffer, command_pool, queue);
    }

    VkImageView CreateImageView(VkDevice logical_device,
                                VkImage image,
                                VkFormat format)
    {
        VkImageViewCreateInfo view_info{};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = image;
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = format;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        VkImageView image_view;
        auto view_create_result =
            vkCreateImageView(logical_device, &view_info, nullptr, &image_view);
        if (view_create_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture image view!");
        }

        return image_view;
    }

    VkSampler CreateImageSampler(VkDevice logical_device,
                                 VkPhysicalDevice physical_device,
                                 VkFilter filter,
                                 VkSamplerAddressMode u_mode,
                                 VkSamplerAddressMode v_mode)
    {
        VkSamplerCreateInfo sampler_info{};
        sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        sampler_info.magFilter = filter;
        sampler_info.minFilter = filter;

        sampler_info.addressModeU = u_mode;
        sampler_info.addressModeV = v_mode;
        sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        sampler_info.anisotropyEnable = VK_TRUE;

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physical_device, &properties);
        sampler_info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

        sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

        sampler_info.unnormalizedCoordinates = VK_FALSE;

        sampler_info.compareEnable = VK_FALSE;
        sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;

        sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        sampler_info.mipLodBias = 0.0f;
        sampler_info.minLod = 0.0f;
        sampler_info.maxLod = 0.0f;

        VkSampler sampler;
        auto sampler_create_result =
            vkCreateSampler(logical_device, &sampler_info, nullptr, &sampler);
        if (sampler_create_result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture sampler!");
        }

        return sampler;
    }
}  // namespace Merlin
