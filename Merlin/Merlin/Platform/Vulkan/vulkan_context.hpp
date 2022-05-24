#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP
#include "Merlin/Render/graphics_context.hpp"
#include "vulkan/vulkan.h"


namespace Merlin
{
    class VulkanContext : public GraphicsContext
    {
        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        static bool deviceIsSuitable(const VkPhysicalDevice& device);
    public:

        VulkanContext();
        ~VulkanContext();
        virtual void Init() override;
        virtual void SwapBuffers() override;
    };
}
#endif