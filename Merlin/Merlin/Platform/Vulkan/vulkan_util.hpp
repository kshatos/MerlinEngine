#ifndef VULKAN_UTIL_HPP
#define VULKAN_UTIL_HPP
#include "vulkan/vulkan.h"
#include <glfw/glfw3.h>
#include <vector>
#include <optional>


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
}
#endif