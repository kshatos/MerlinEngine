#ifndef VULKAN_INSTANCE_HPP
#define VULKAN_INSTANCE_HPP
#include "vulkan/vulkan.h"
#include <vector>


namespace Merlin
{
    struct VulkanInstance
    {
        VkInstance handle;

        VulkanInstance(std::vector<const char*> validationLayerNames);
    };
}
#endif