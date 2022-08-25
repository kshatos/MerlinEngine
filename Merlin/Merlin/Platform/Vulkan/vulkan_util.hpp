#ifndef VULKAN_UTIL_HPP
#define VULKAN_UTIL_HPP
#include <glfw/glfw3.h>

#include <optional>
#include <stdexcept>
#include <vector>

#include "Merlin/Platform/Vulkan/vulkan_texture2d.hpp"
#include "vulkan/vulkan.h"

namespace Merlin
{
    struct VulkanPhysicalDeviceInfo
    {
        VkPhysicalDevice handle;

        VkPhysicalDeviceProperties device_properties;
        VkPhysicalDeviceFeatures device_features;

        std::vector<VkExtensionProperties> extension_properties;

        std::vector<VkQueueFamilyProperties> queue_family_properties;

        VkSurfaceCapabilitiesKHR surface_capabilities;
        std::vector<VkSurfaceFormatKHR> surface_formats;
        std::vector<VkPresentModeKHR> surface_present_modes;

        VulkanPhysicalDeviceInfo()
            : handle(VK_NULL_HANDLE)
            , device_properties()
            , device_features()
            , extension_properties()
            , queue_family_properties()
            , surface_capabilities()
            , surface_formats()
            , surface_present_modes()
        {
        }

        VulkanPhysicalDeviceInfo(VkPhysicalDevice device_handle,
                                 VkInstance instance,
                                 VkSurfaceKHR surface);
    };

    float EvaluateDevice(const VulkanPhysicalDeviceInfo& info);

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
    };

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device,
                                         VkSurfaceKHR surface);

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device,
                                                  VkSurfaceKHR surface);

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& available_formats);

    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& available_present_modes);

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                                GLFWwindow* window);

    VkCommandBuffer BeginSingleTimeCommands(VkDevice logical_device,
                                            VkCommandPool command_pool);

    void EndSingleTimeCommands(VkDevice logical_device,
                               VkCommandBuffer command_buffer,
                               VkCommandPool command_pool,
                               VkQueue queue);

    uint32_t FindMemoryType(uint32_t type_filter,
                            VkMemoryPropertyFlags properties,
                            VkPhysicalDevice physical_device);

    void CreateBuffer(VkDevice device,
                      VkPhysicalDevice physical_device,
                      VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer& buffer,
                      VkDeviceMemory& buffer_memory);

    void CopyBuffer(VkQueue queue,
                    VkDevice device,
                    VkCommandPool command_buffer,
                    VkBuffer src_buffer,
                    VkBuffer dst_buffer,
                    VkDeviceSize size);

    VkFormat GetVkChannelFormat(const uint32_t& channel_count);

    VkFilter GetVkFilterMode(TextureFilterMode filter_mode);

    VkSamplerAddressMode GetVkAdressMode(TextureWrapMode wrap_mode);

    void CreateImage(VkDevice logical_device,
                     VkPhysicalDevice physical_device,
                     uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage& image,
                     VkDeviceMemory& image_memory);

    void CopyBufferToImage(VkDevice logical_device,
                           VkCommandPool command_pool,
                           VkQueue queue,
                           VkBuffer buffer,
                           VkImage image,
                           uint32_t width,
                           uint32_t height);

    void TransitionImageLayout(VkDevice logical_device,
                               VkCommandPool command_pool,
                               VkQueue queue,
                               VkImage image,
                               VkFormat format,
                               VkImageLayout old_layout,
                               VkImageLayout new_layout);

    VkImageView CreateImageView(VkDevice logical_device,
                                VkImage image,
                                VkFormat format);

    VkSampler CreateImageSampler(VkDevice logical_device,
                                 VkPhysicalDevice physical_device,
                                 VkFilter filter,
                                 VkSamplerAddressMode u_mode,
                                 VkSamplerAddressMode v_mode);
}  // namespace Merlin
#endif