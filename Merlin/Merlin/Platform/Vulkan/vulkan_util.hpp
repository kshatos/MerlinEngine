#ifndef VULKAN_UTIL_HPP
#define VULKAN_UTIL_HPP
#include "Merlin/Platform/Vulkan/vulkan_texture2d.hpp"
#include "vulkan/vulkan.h"
#include <glfw/glfw3.h>
#include <vector>
#include <optional>
#include <stdexcept>


namespace Merlin
{
    struct VulkanPhysicalDeviceInfo
    {
        VkPhysicalDevice handle;

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        std::vector<VkExtensionProperties> extensionProperties;

        std::vector<VkQueueFamilyProperties> queueFamilyProperties;

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> surfacePresentModes;

        VulkanPhysicalDeviceInfo() :
            handle(VK_NULL_HANDLE),
            deviceProperties(),
            deviceFeatures(),
            extensionProperties(),
            queueFamilyProperties(),
            surfaceCapabilities(),
            surfaceFormats(),
            surfacePresentModes()
        {}

        VulkanPhysicalDeviceInfo(
            VkPhysicalDevice deviceHandle,
            VkInstance instance,
            VkSurfaceKHR surface);
    };

    float evaluateDevice(const VulkanPhysicalDeviceInfo& info);

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
    };

    QueueFamilyIndices findQueueFamilies(
        VkPhysicalDevice device,
        VkSurfaceKHR surface);

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(
        VkPhysicalDevice device,
        VkSurfaceKHR surface);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(
        const VkSurfaceCapabilitiesKHR& capabilities,
        GLFWwindow* window);

    VkCommandBuffer BeginSingleTimeCommands(
        VkDevice logicalDevice,
        VkCommandPool commandPool);

    void EndSingleTimeCommands(
        VkDevice logicalDevice,
        VkCommandBuffer commandBuffer,
        VkCommandPool commandPool,
        VkQueue queue);

    uint32_t findMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties,
        VkPhysicalDevice physicalDevice);

    void createBuffer(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& buffer,
        VkDeviceMemory& bufferMemory);

    void copyBuffer(
        VkQueue queue,
        VkDevice device,
        VkCommandPool commandBuffer,
        VkBuffer srcBuffer,
        VkBuffer dstBuffer,
        VkDeviceSize size);

    VkFormat  GetVkChannelFormat(const uint32_t& channel_count);

    VkFilter  GetVkFilterMode(TextureFilterMode filterMode);

    VkSamplerAddressMode GetVkAdressMode(TextureWrapMode wrapMode);

    void createImage(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        uint32_t width,
        uint32_t height,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkImage& image,
        VkDeviceMemory& imageMemory);

    void copyBufferToImage(
        VkDevice logicalDevice,
        VkCommandPool commandPool,
        VkQueue queue,
        VkBuffer buffer,
        VkImage image,
        uint32_t width,
        uint32_t height);

    void transitionImageLayout(
        VkDevice logicalDevice,
        VkCommandPool commandPool,
        VkQueue queue,
        VkImage image,
        VkFormat format,
        VkImageLayout oldLayout,
        VkImageLayout newLayout);

    VkImageView createImageView(
        VkDevice logical_device,
        VkImage image,
        VkFormat format);

    VkSampler createImageSampler(
        VkDevice logicalDevice,
        VkPhysicalDevice physicalDevice,
        VkFilter filter,
        VkSamplerAddressMode uMode,
        VkSamplerAddressMode vMode);
}
#endif