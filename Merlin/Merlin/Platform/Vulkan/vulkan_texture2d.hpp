#ifndef VULKAN_TEXTURE2D_HPP
#define VULKAN_TEXTURE2D_HPP
#include "Merlin/Render/texture2d.hpp"
#include "Merlin/Render/texture2d_data.hpp"
#include "vulkan/vulkan.h"

namespace Merlin
{
    class VulkanTexture2D : public Texture2D
    {
        VkDevice m_logical_device;

    public:
        VkImageView m_texture_view;
        VkSampler m_texture_sampler;
        VkImage m_texture_image;
        VkDeviceMemory m_texture_image_memory;
        VulkanTexture2D(VkDevice logical_device,
                        VkPhysicalDevice physical_device,
                        VkCommandPool command_pool,
                        VkQueue queue,
                        const Texture2DData& texture_data,
                        Texture2DProperties props = Texture2DProperties());
        ~VulkanTexture2D();
    };
}  // namespace Merlin
#endif