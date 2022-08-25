#ifndef VULKAN_INDEX_BUFFER_HPP
#define VULKAN_INDEX_BUFFER_HPP
#include "Merlin/Render/index_buffer.hpp"
#include "vulkan/vulkan.h"

namespace Merlin
{
    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        uint32_t m_index_count;
        VkDevice m_logical_device;
        VkBuffer m_index_buffer;
        VkDeviceMemory m_index_buffer_memory;

        VulkanIndexBuffer(uint32_t* indices,
                          uint32_t index_count,
                          VkDevice logical_device,
                          VkPhysicalDevice physical_device,
                          VkQueue queue,
                          VkCommandPool command_buffer);
        ~VulkanIndexBuffer();

        inline uint32_t GetCount() { return m_index_count; }

        void BindCommand(VkCommandBuffer command_buffer);
    };

}  // namespace Merlin
#endif
