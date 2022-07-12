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
            VkDevice logicalDevice,
            VkPhysicalDevice physicalDevice);
        ~VulkanVertexBuffer();

        void BindCommand(VkCommandBuffer commandBuffer);
    };
}
#endif