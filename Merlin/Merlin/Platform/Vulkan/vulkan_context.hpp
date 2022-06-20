#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include "Merlin/Render/graphics_context.hpp"
#include "vulkan/vulkan.h"
#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_logical_device.hpp"
#include <memory>


namespace Merlin
{

    class VulkanContext
    {
    public:
        std::shared_ptr<VulkanInstance> instance;
        std::shared_ptr<VulkanPhysicalDevice> physicalDevice;
        std::shared_ptr<VulkanLogicalDevice> logicalDevice;
        VkSurfaceKHR surface;

        VulkanContext(void* window);

    private:
        void CreateSurface(GLFWwindow* window);
        void PickPhysicalDevice();
    };
}
#endif