#include "Merlin/Platform/Vulkan/vulkan_index_buffer.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"


namespace Merlin
{
    VulkanIndexBuffer::VulkanIndexBuffer(
        uint32_t* indices,
        uint32_t index_count,
        VkDevice logicalDevice,
        VkPhysicalDevice physicalDevice,
        VkQueue queue,
        VkCommandPool commandBuffer)
    {
        VkDeviceSize bufferSize = sizeof(uint32_t) * index_count;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(
            logicalDevice,
            physicalDevice,
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void* data;
        vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices, (size_t)bufferSize);
        vkUnmapMemory(logicalDevice, stagingBufferMemory);

        createBuffer(
            logicalDevice,
            physicalDevice,
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_indexBuffer,
            m_indexBufferMemory);

        copyBuffer(
            queue,
            logicalDevice,
            commandBuffer,
            stagingBuffer,
            m_indexBuffer,
            bufferSize);

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

        m_logicalDevice = logicalDevice;
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        vkDestroyBuffer(m_logicalDevice, m_indexBuffer, nullptr);
        vkFreeMemory(m_logicalDevice, m_indexBufferMemory, nullptr);
    }

    void VulkanIndexBuffer::BindCommand(VkCommandBuffer commandBuffer)
    {
        vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
}
