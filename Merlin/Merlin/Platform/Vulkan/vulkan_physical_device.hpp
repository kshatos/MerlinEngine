#ifndef VULKAN_PHYSICAL_DEVICE
#define VULKAN_PHYSICAL_DEVICE
#include<vulkan/vulkan.h>
#include <vector>

namespace Merlin
{
    struct VulkanPhysicalDevice
    {
        VkPhysicalDevice handle;

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        std::vector<VkExtensionProperties> extensionProperties;

        std::vector<VkQueueFamilyProperties> queueFamilyProperties;

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> surfacePresentModes;


        VulkanPhysicalDevice(
            VkPhysicalDevice deviceHandle,
            VkInstance instance,
            VkSurfaceKHR surface);
    };
}
#endif