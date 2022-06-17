#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include "Merlin/Render/graphics_context.hpp"
#include "vulkan/vulkan.h"



namespace Merlin
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
    };

    class VulkanContext : public GraphicsContext
    {
        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_logicalDevice;
        VkQueue m_graphicsQueue;
        QueueFamilyIndices m_indices;
        VkSurfaceKHR m_surface;

        bool deviceIsSuitable(const VkPhysicalDevice& device);
        void PickPhysicalDevice();
        void CreateInstance();
        void CreateSurface(GLFWwindow* window);
        VkDeviceQueueCreateInfo GetDeviceQueueInfo();
        void CreateLogicalDevice();

    public:

        VulkanContext(GLFWwindow* window);
        ~VulkanContext();
        virtual void Init() override;
        virtual void SwapBuffers() override;
    };
}
#endif