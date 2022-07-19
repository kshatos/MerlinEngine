#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include <set>
#include <stdexcept>
#include <algorithm>


namespace Merlin
{
    //////////////////////////////
    // PHYSICAL DEVICE
    //////////////////////////////
    VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(
        VkPhysicalDevice deviceHandle,
        VkInstance instance,
        VkSurfaceKHR surface)
    {
        handle = deviceHandle;

        // Base details
        vkGetPhysicalDeviceProperties(deviceHandle, &deviceProperties);
        vkGetPhysicalDeviceFeatures(deviceHandle, &deviceFeatures);

        // Extension support
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(
            deviceHandle, nullptr, &extensionCount, nullptr);

        extensionProperties.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(
            deviceHandle, nullptr, &extensionCount, extensionProperties.data());

        // Queue families
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            deviceHandle, &queueFamilyCount, nullptr);
        queueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            deviceHandle, &queueFamilyCount, queueFamilyProperties.data());

        // Surface support
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            deviceHandle, surface, &surfaceCapabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            deviceHandle, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                deviceHandle, surface, &formatCount, surfaceFormats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            deviceHandle, surface, &presentModeCount, nullptr);
        if (presentModeCount > 0)
        {
            surfacePresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                deviceHandle, surface, &presentModeCount, surfacePresentModes.data());
        }
    }

    float evaluateDevice(const VulkanPhysicalDeviceInfo& info)
    {
        return 1.0f;
    }

    //////////////////////////////
    // LOGICAL DEVICE
    //////////////////////////////
    QueueFamilyIndices findQueueFamilies(
        VkPhysicalDevice device,
        VkSurfaceKHR surface)
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            device, &queueFamilyCount, queueFamilies.data());

        QueueFamilyIndices indices;
        int i = 0;
        for (const auto& familyProps : queueFamilies)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport)
            {
                indices.presentFamily = i;
            }

            if (familyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;
            i++;
        }

        return indices;
    }

    //////////////////////////////
    // SWAP CHAIN
    //////////////////////////////
    SwapChainSupportDetails querySwapChainSupport(
        VkPhysicalDevice device,
        VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &presentModeCount, nullptr);
        if (presentModeCount > 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        // Check for prefered
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        // Use whatevers available
        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        // Check for prefered
        for (const auto& avialabeMode : availablePresentModes)
        {
            if (avialabeMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return avialabeMode;
            }
        }

        // Guaranteed otherwise
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(
        const VkSurfaceCapabilitiesKHR& capabilities,
        GLFWwindow* window)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        actualExtent.width = std::clamp(actualExtent.width,
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height,

            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);

        return actualExtent;
    }


    //////////////////////////////
    // BUFFERS
    //////////////////////////////
    uint32_t findMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties,
        VkPhysicalDevice physicalDevice)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if (typeFilter & (1 << i) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }
        throw std::runtime_error("failed to find suitable memory type!");

        return 0;
    }

    void createBuffer(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties, 
        VkBuffer& buffer,
        VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(
            memRequirements.memoryTypeBits, properties, physicalDevice);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    void copyBuffer(
        VkQueue queue,
        VkDevice device,
        VkCommandPool commandPool,
        VkBuffer srcBuffer,
        VkBuffer dstBuffer,
        VkDeviceSize size)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        {
            VkBufferCopy copyRegion{};
            copyRegion.srcOffset = 0;
            copyRegion.dstOffset = 0;
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
        }
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }

}
