#include "Merlin/Platform/Vulkan/vulkan_vertex_buffer.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include <stdexcept>


namespace Merlin
{
    VulkanVertexBuffer::VulkanVertexBuffer(
        float* vertices,
        size_t size,
        BufferLayout layout,
        VkDevice logicalDevice,
        VkPhysicalDevice physicalDevice,
        VkQueue queue,
        VkCommandPool commandBuffer)
    {
        VkDeviceSize bufferSize = sizeof(float) * size;

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
        memcpy(data, vertices, bufferSize);
        vkUnmapMemory(logicalDevice, stagingBufferMemory);

        createBuffer(
            logicalDevice,
            physicalDevice,
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_vertexBuffer,
            m_vertexBufferMemory);

        copyBuffer(
            queue,
            logicalDevice,
            commandBuffer,
            stagingBuffer,
            m_vertexBuffer,
            bufferSize);

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

        m_logicalDevice = logicalDevice;
        m_layout = layout;
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        vkDestroyBuffer(m_logicalDevice, m_vertexBuffer, nullptr);
        vkFreeMemory(m_logicalDevice, m_vertexBufferMemory, nullptr);
    }

    void VulkanVertexBuffer::BindCommand(VkCommandBuffer commandBuffer)
    {
        VkBuffer vertexBuffers[] = { m_vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    }

}