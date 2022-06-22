#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include <set>
#include <stdexcept>
#include <algorithm>


namespace Merlin
{
    //////////////////////////////
    // PHYSICAL DEVICE
    //////////////////////////////
    VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(
        VkPhysicalDevice deviceHandle,
        VkInstance instance,
        VkSurfaceKHR surface)
    {
        handle = deviceHandle;

        // Base details
        vkGetPhysicalDeviceProperties(deviceHandle, &deviceProperties);
        vkGetPhysicalDeviceFeatures(deviceHandle, &deviceFeatures);

        // Extension support
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(
            deviceHandle, nullptr, &extensionCount, nullptr);

        extensionProperties.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(
            deviceHandle, nullptr, &extensionCount, extensionProperties.data());

        // Queue families
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            deviceHandle, &queueFamilyCount, nullptr);
        queueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            deviceHandle, &queueFamilyCount, queueFamilyProperties.data());

        // Surface support
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            deviceHandle, surface, &surfaceCapabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            deviceHandle, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                deviceHandle, surface, &formatCount, surfaceFormats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            deviceHandle, surface, &presentModeCount, nullptr);
        if (presentModeCount > 0)
        {
            surfacePresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                deviceHandle, surface, &presentModeCount, surfacePresentModes.data());
        }
    }

    float evaluateDevice(const VulkanPhysicalDeviceInfo& info)
    {
        return 1.0f;
    }

    //////////////////////////////
    // LOGICAL DEVICE
    //////////////////////////////
    QueueFamilyIndices findQueueFamilies(
        VkPhysicalDevice device,
        VkSurfaceKHR surface)
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            device, &queueFamilyCount, queueFamilies.data());

        QueueFamilyIndices indices;
        int i = 0;
        for (const auto& familyProps : queueFamilies)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport)
            {
                indices.presentFamily = i;
            }

            if (familyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;
            i++;
        }

        return indices;
    }

    //////////////////////////////
    // SWAP CHAIN
    //////////////////////////////
    SwapChainSupportDetails querySwapChainSupport(
        VkPhysicalDevice device,
        VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &presentModeCount, nullptr);
        if (presentModeCount > 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        // Check for prefered
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        // Use whatevers available
        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        // Check for prefered
        for (const auto& avialabeMode : availablePresentModes)
        {
            if (avialabeMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return avialabeMode;
            }
        }

        // Guaranteed otherwise
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(
        const VkSurfaceCapabilitiesKHR& capabilities,
        GLFWwindow* window)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        actualExtent.width = std::clamp(actualExtent.width,
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height,

            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);

        return actualExtent;
    }

}
