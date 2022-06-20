#include <stdexcept>
#include <vector>
#include "vulkan_context.hpp"
#include <GLFW/glfw3.h>


namespace Merlin
{

    VulkanContext::VulkanContext(void* window)
    {
        std::vector<const char*> deviceExtensions;
        std::vector<const char*> validationLayerNames;

        auto glfwWindow = (GLFWwindow*)window;

        instance = std::make_shared<VulkanInstance>(
            validationLayerNames);
        
        CreateSurface(glfwWindow);
        PickPhysicalDevice();

        logicalDevice = std::make_shared<VulkanLogicalDevice>(
            *physicalDevice,
            surface,
            deviceExtensions,
            validationLayerNames);
    }

    void VulkanContext::CreateSurface(GLFWwindow* window)
    {
        auto surfaceCreateResult = glfwCreateWindowSurface(
            instance->handle, window, nullptr, &surface);
        if (surfaceCreateResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void VulkanContext::PickPhysicalDevice()
    {
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(
            instance->handle, &deviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(
            instance->handle, &deviceCount, devices.data());

        float bestScore = -std::numeric_limits<float>::infinity();
        VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;
        for (const auto& device : devices)
        {
            float score = 1.0f;//evaluationFunction(device);
            if (score > bestScore)
            {
                bestScore = score;
                selectedDevice = device;
            }
        }

        if (selectedDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Unable to find acceptable GPU!");
        }

        physicalDevice = std::make_shared<VulkanPhysicalDevice>(
            selectedDevice,
            instance->handle,
            surface);
    }

}
