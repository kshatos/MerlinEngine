#include "Merlin/Platform/Vulkan/vulkan_index_buffer.hpp"

#include "Merlin/Platform/Vulkan/vulkan_util.hpp"

namespace Merlin
{
    VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices,
                                         uint32_t index_count,
                                         VkDevice logical_device,
                                         VkPhysicalDevice physical_device,
                                         VkQueue queue,
                                         VkCommandPool command_buffer)
        : m_index_count(0)
        , m_logical_device(VK_NULL_HANDLE)
        , m_index_buffer(VK_NULL_HANDLE)
        , m_index_buffer_memory(VK_NULL_HANDLE)
    {
        VkDeviceSize buffer_size = sizeof(uint32_t) * index_count;

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
        memcpy(data, indices, (size_t)buffer_size);
        vkUnmapMemory(logical_device, staging_buffer_memory);

        CreateBuffer(
            logical_device,
            physical_device,
            buffer_size,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_index_buffer,
            m_index_buffer_memory);

        CopyBuffer(queue,
                   logical_device,
                   command_buffer,
                   staging_buffer,
                   m_index_buffer,
                   buffer_size);

        vkDestroyBuffer(logical_device, staging_buffer, nullptr);
        vkFreeMemory(logical_device, staging_buffer_memory, nullptr);

        m_logical_device = logical_device;
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        vkDestroyBuffer(m_logical_device, m_index_buffer, nullptr);
        vkFreeMemory(m_logical_device, m_index_buffer_memory, nullptr);
    }

    void VulkanIndexBuffer::BindCommand(VkCommandBuffer commandBuffer)
    {
        vkCmdBindIndexBuffer(
            commandBuffer, m_index_buffer, 0, VK_INDEX_TYPE_UINT32);
    }
}  // namespace Merlin
