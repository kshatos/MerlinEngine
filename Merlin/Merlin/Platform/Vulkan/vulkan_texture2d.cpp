#include "Merlin/Platform/Vulkan/vulkan_texture2d.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include "Merlin/Core/logger.hpp"
#include <stdexcept>


namespace Merlin
{
    VulkanTexture2D::VulkanTexture2D(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkCommandPool command_pool,
        VkQueue queue,
        const Texture2DData& texture_data,
        Texture2DProperties props) :
        m_textureImage(VK_NULL_HANDLE),
        m_textureImageMemory(VK_NULL_HANDLE),
        m_textureView(VK_NULL_HANDLE),
        m_textureSampler(VK_NULL_HANDLE),
        m_logicalDevice(VK_NULL_HANDLE)
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

        m_textureView = createImageView(
            logical_device,
            m_textureImage,
            format);

        m_textureSampler = createImageSampler(
            logical_device,
            physical_device,
            GetVkFilterMode(props.filter_mode),
            GetVkAdressMode(props.s_wrap_mode),
            GetVkAdressMode(props.t_wrap_mode));

        m_logicalDevice = logical_device;
    }

    VulkanTexture2D::~VulkanTexture2D()
    {
        vkDestroySampler(m_logicalDevice, m_textureSampler, nullptr);
        vkDestroyImageView(m_logicalDevice, m_textureView, nullptr);
        vkDestroyImage(m_logicalDevice, m_textureImage, nullptr);
        vkFreeMemory(m_logicalDevice, m_textureImageMemory, nullptr);
    }
}
