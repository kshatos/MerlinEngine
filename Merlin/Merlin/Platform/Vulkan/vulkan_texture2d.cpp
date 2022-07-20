#include "Merlin/Platform/Vulkan/vulkan_texture2d.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include "Merlin/Core/logger.hpp"
#include <stdexcept>


namespace Merlin
{
    /*
    TODO:
    Find a better way of figuring out available image
    formats instead of hardcoding
    */
    VkFormat  GetVkChannelFormat(const uint32_t& channel_count)
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

    VulkanTexture2D::VulkanTexture2D(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkCommandPool command_pool,
        VkQueue queue,
        const Texture2DData& texture_data,
        Texture2DProperties props)
    {
        size_t texWidth = texture_data.GetXResolution();
        size_t texHeight = texture_data.GetYResolution();
        size_t texChannels = texture_data.GetChannelCount();
        size_t imageSize = texWidth * texHeight * texChannels;
        auto format = GetVkChannelFormat(texChannels);

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(
            logical_device,
            physical_device,
            imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void* data;
        vkMapMemory(logical_device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, (void*)texture_data.GetDataPointer(), imageSize);
        vkUnmapMemory(logical_device, stagingBufferMemory);

        createImage(
            logical_device,
            physical_device,
            texWidth,
            texHeight,
            format,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_textureImage,
            m_textureImageMemory);

        transitionImageLayout(
            logical_device,
            command_pool,
            queue,
            m_textureImage,
            format,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        copyBufferToImage(
            logical_device,
            command_pool,
            queue,
            stagingBuffer,
            m_textureImage,
            static_cast<uint32_t>(texWidth),
            static_cast<uint32_t>(texHeight));

        vkDestroyBuffer(logical_device, stagingBuffer, nullptr);
        vkFreeMemory(logical_device, stagingBufferMemory, nullptr);

        transitionImageLayout(
            logical_device,
            command_pool,
            queue,
            m_textureImage,
            format,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        m_logicalDevice = logical_device;
    }


    VulkanTexture2D::~VulkanTexture2D()
    {
        vkDestroyImage(m_logicalDevice, m_textureImage, nullptr);
        vkFreeMemory(m_logicalDevice, m_textureImageMemory, nullptr);
    }
}
