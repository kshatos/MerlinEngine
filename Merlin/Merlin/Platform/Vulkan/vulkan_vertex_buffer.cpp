#include "Merlin/Platform/Vulkan/vulkan_vertex_buffer.hpp"

#include <stdexcept>

#include "Merlin/Platform/Vulkan/vulkan_util.hpp"

namespace Merlin
{
    VulkanVertexBuffer::VulkanVertexBuffer(float* vertices,
                                           size_t size,
                                           BufferLayout layout,
                                           VkDevice logical_device,
                                           VkPhysicalDevice physical_device,
                                           VkQueue queue,
                                           VkCommandPool command_buffer)
    {
        VkDeviceSize buffer_size = sizeof(float) * size;

        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        CreateBuffer(logical_device,
                     physical_device,
                     buffer_size,
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     staging_buffer,
                     staging_buffer_memory);

        void* data;
        vkMapMemory(
            logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
        memcpy(data, vertices, buffer_size);
        vkUnmapMemory(logical_device, staging_buffer_memory);

        CreateBuffer(logical_device,
                     physical_device,
                     buffer_size,
                     VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                     m_vertex_buffer,
                     m_vertex_buffer_memory);

        CopyBuffer(queue,
                   logical_device,
                   command_buffer,
                   staging_buffer,
                   m_vertex_buffer,
                   buffer_size);

        vkDestroyBuffer(logical_device, staging_buffer, nullptr);
        vkFreeMemory(logical_device, staging_buffer_memory, nullptr);

        m_logical_device = logical_device;
        m_layout = layout;
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        vkDestroyBuffer(m_logical_device, m_vertex_buffer, nullptr);
        vkFreeMemory(m_logical_device, m_vertex_buffer_memory, nullptr);
    }

    void VulkanVertexBuffer::BindCommand(VkCommandBuffer command_buffer)
    {
        VkBuffer vertex_buffers[] = {m_vertex_buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
    }

}  // namespace Merlin