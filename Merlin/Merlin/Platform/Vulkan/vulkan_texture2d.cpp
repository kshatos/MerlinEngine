#include "Merlin/Platform/Vulkan/vulkan_texture2d.hpp"

#include <stdexcept>

#include "Merlin/Core/logger.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"

namespace Merlin
{
    VulkanTexture2D::VulkanTexture2D(VkDevice logical_device,
                                     VkPhysicalDevice physical_device,
                                     VkCommandPool command_pool,
                                     VkQueue queue,
                                     const Texture2DData& texture_data,
                                     Texture2DProperties props)
        : m_texture_image(VK_NULL_HANDLE)
        , m_texture_image_memory(VK_NULL_HANDLE)
        , m_texture_view(VK_NULL_HANDLE)
        , m_texture_sampler(VK_NULL_HANDLE)
        , m_logical_device(VK_NULL_HANDLE)
    {
        size_t tex_width = texture_data.GetXResolution();
        size_t tex_height = texture_data.GetYResolution();
        size_t tex_channels = texture_data.GetChannelCount();
        size_t image_size = tex_width * tex_height * tex_channels;
        auto format = GetVkChannelFormat(tex_channels);

        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        CreateBuffer(logical_device,
                     physical_device,
                     image_size,
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     staging_buffer,
                     staging_buffer_memory);

        void* data;
        vkMapMemory(
            logical_device, staging_buffer_memory, 0, image_size, 0, &data);
        memcpy(data, (void*)texture_data.GetDataPointer(), image_size);
        vkUnmapMemory(logical_device, staging_buffer_memory);

        CreateImage(
            logical_device,
            physical_device,
            tex_width,
            tex_height,
            format,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_texture_image,
            m_texture_image_memory);

        TransitionImageLayout(logical_device,
                              command_pool,
                              queue,
                              m_texture_image,
                              format,
                              VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        CopyBufferToImage(logical_device,
                          command_pool,
                          queue,
                          staging_buffer,
                          m_texture_image,
                          static_cast<uint32_t>(tex_width),
                          static_cast<uint32_t>(tex_height));

        vkDestroyBuffer(logical_device, staging_buffer, nullptr);
        vkFreeMemory(logical_device, staging_buffer_memory, nullptr);

        TransitionImageLayout(logical_device,
                              command_pool,
                              queue,
                              m_texture_image,
                              format,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        m_texture_view =
            CreateImageView(logical_device, m_texture_image, format);

        m_texture_sampler =
            CreateImageSampler(logical_device,
                               physical_device,
                               GetVkFilterMode(props.filter_mode),
                               GetVkAdressMode(props.s_wrap_mode),
                               GetVkAdressMode(props.t_wrap_mode));

        m_logical_device = logical_device;
    }

    VulkanTexture2D::~VulkanTexture2D()
    {
        vkDestroySampler(m_logical_device, m_texture_sampler, nullptr);
        vkDestroyImageView(m_logical_device, m_texture_view, nullptr);
        vkDestroyImage(m_logical_device, m_texture_image, nullptr);
        vkFreeMemory(m_logical_device, m_texture_image_memory, nullptr);
    }
}  // namespace Merlin
