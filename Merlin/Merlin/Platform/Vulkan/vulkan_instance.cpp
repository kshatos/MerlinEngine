#include "vulkan_instance.hpp"
#include "GLFW/glfw3.h"
#include <stdexcept>


namespace Merlin
{
    bool validationLayersSupported(
        std::vector<const char*> layerNames)
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : layerNames)
        {
            bool layerFound = false;
            for (const auto& layerProps : availableLayers)
            {
                if (strcmp(layerName, layerProps.layerName))
                {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound)
                return false;
        }

        return true;
    }


    VulkanInstance::VulkanInstance(std::vector<const char*> validationLayerNames)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Merlin Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        if (validationLayersSupported(validationLayerNames))
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayerNames.size());
            createInfo.ppEnabledLayerNames = validationLayerNames.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        auto instanceCreateResult = vkCreateInstance(
            &createInfo, nullptr, &handle);
        if (instanceCreateResult != VK_SUCCESS)
        {
            throw std::runtime_error("Unable to create vulkan instance!");
        }
    }
}