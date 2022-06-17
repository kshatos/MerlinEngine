#ifndef VULKAN_LOGICAL_DEVICE
#define VULKAN_LOGICAL_DEVICE
#include <vulkan/vulkan.h>
#include "vulkan_physical_device.hpp"


namespace Merlin
{
    struct VulkanLogicalDevice
    {
        VkDevice handle;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VulkanLogicalDevice(
            VulkanPhysicalDevice physicalDevice,
            VkSurfaceKHR surface,
            std::vector<const char*> deviceExtensions,
            std::vector<const char*> validationLayerNames);

        ~VulkanLogicalDevice();
    };

}
#endif