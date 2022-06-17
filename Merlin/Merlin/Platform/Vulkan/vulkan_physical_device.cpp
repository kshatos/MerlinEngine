#include "vulkan_physical_device.hpp"


namespace Merlin
{
    VulkanPhysicalDevice::VulkanPhysicalDevice(
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
}
