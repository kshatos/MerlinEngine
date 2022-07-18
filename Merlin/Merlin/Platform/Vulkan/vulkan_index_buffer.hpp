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
        VkDevice m_logicalDevice;
        VkBuffer m_indexBuffer;
        VkDeviceMemory m_indexBufferMemory;

        VulkanIndexBuffer(
            uint32_t* indices,
            uint32_t index_count,
            VkDevice logicalDevice,
            VkPhysicalDevice physicalDevice,
            VkQueue queue,
            VkCommandPool commandBuffer);
        ~VulkanIndexBuffer();

        inline uint32_t GetCount() { return m_index_count; }

        void BindCommand(VkCommandBuffer commandBuffer);
    };

}
#endif
