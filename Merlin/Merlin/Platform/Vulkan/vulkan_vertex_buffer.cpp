#include "Merlin/Platform/Vulkan/vulkan_vertex_buffer.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include <stdexcept>


namespace Merlin
{
    VulkanVertexBuffer::VulkanVertexBuffer(
        float* vertices,
        size_t size,
        VkDevice logicalDevice,
        VkPhysicalDevice physicalDevice)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(float) * size;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        auto createResult = vkCreateBuffer(
            logicalDevice, &bufferInfo, nullptr, &m_vertexBuffer);
        if (createResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(
            logicalDevice, m_vertexBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            physicalDevice);

        auto allocateResult = vkAllocateMemory(
            logicalDevice, &allocInfo, nullptr, &m_vertexBufferMemory);
        if (allocateResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(logicalDevice, m_vertexBuffer, m_vertexBufferMemory, 0);

        void* data;
        vkMapMemory(logicalDevice, m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices, size);
        vkUnmapMemory(logicalDevice, m_vertexBufferMemory);

        m_logicalDevice = logicalDevice;
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