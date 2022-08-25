#ifndef VULKAN_VERTEX_BUFFER_HPP
#define VULKAN_VERTEX_BUFFER_HPP
#include "Merlin/Render/vertex_buffer.hpp"
#include "vulkan/vulkan.h"

namespace Merlin
{
    class VulkanVertexBuffer : public VertexBuffer
    {
        VkDevice m_logical_device;

    public:
        VkBuffer m_vertex_buffer;
        VkDeviceMemory m_vertex_buffer_memory;

        VulkanVertexBuffer(float* vertices,
                           size_t size,
                           BufferLayout layout,
                           VkDevice logical_device,
                           VkPhysicalDevice physical_device,
                           VkQueue queue,
                           VkCommandPool command_buffer);
        ~VulkanVertexBuffer();

        void BindCommand(VkCommandBuffer command_buffer);
    };
}  // namespace Merlin
#endif