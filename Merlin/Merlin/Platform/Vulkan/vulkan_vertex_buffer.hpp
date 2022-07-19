#ifndef VULKAN_VERTEX_BUFFER_HPP
#define VULKAN_VERTEX_BUFFER_HPP
#include "Merlin/Render/vertex_buffer.hpp"
#include "vulkan/vulkan.h"


namespace Merlin
{
    class VulkanVertexBuffer : public VertexBuffer
    {
        VkDevice m_logicalDevice;
    public:
        VkBuffer m_vertexBuffer;
        VkDeviceMemory m_vertexBufferMemory;

        VulkanVertexBuffer(
            float* vertices,
            size_t size,
            BufferLayout layout,
            VkDevice logicalDevice,
            VkPhysicalDevice physicalDevice,
            VkQueue queue,
            VkCommandPool commandBuffer);
        ~VulkanVertexBuffer();

        void BindCommand(VkCommandBuffer commandBuffer);
    };
}
#endif