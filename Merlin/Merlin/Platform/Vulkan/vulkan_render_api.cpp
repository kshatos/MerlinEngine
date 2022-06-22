#include "Merlin/Platform/Vulkan/vulkan_render_api.hpp"
#include "Merlin/Platform/Vulkan/vulkan_util.hpp"
#include <stdexcept>
#include <set>


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

    // RENDER API

    VulkanRenderAPI::~VulkanRenderAPI()
    {
        vkDestroyDevice(logicalDevice, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    void VulkanRenderAPI::Init(void* windowPointer)
    {
        window = (GLFWwindow*)windowPointer;
        CreateInstance();
        CreateSurface();
        PickPhysicalDevice();
    }

    void VulkanRenderAPI::SetViewport(
        uint32_t x,
        uint32_t y,
        uint32_t width,
        uint32_t height)
    {
    }

    void VulkanRenderAPI::SetClearColor(const glm::vec4& color)
    {
    }

    void VulkanRenderAPI::Clear()
    {
    }

    void VulkanRenderAPI::DrawTriangles(
        const std::shared_ptr<VertexArray>& vertex_array)
    {
    }


    // SETUP
    void VulkanRenderAPI::CreateInstance()
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
            &createInfo, nullptr, &instance);
        if (instanceCreateResult != VK_SUCCESS)
        {
            throw std::runtime_error("Unable to create vulkan instance!");
        }
    }

    void VulkanRenderAPI::CreateSurface()
    {
        auto surfaceCreateResult = glfwCreateWindowSurface(
            instance, window, nullptr, &surface);
        if (surfaceCreateResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void VulkanRenderAPI::PickPhysicalDevice()
    {
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(
            instance, &deviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(
            instance, &deviceCount, devices.data());

        float bestScore = -std::numeric_limits<float>::infinity();
        VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;
        for (const auto& device : devices)
        {
            VulkanPhysicalDeviceInfo deviceInfo(device, instance, surface);
            float score = evaluateDevice(deviceInfo);
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

        physicalDevice = selectedDevice;
    }

    void VulkanRenderAPI::CreateLogicalDevice()
    {
        auto queueIndices = findQueueFamilies(physicalDevice, surface);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies =
        {
            queueIndices.graphicsFamily.value(),
            queueIndices.presentFamily.value()
        };

        float queuePriority = 1.0f;
        for (auto& queueFamilyIndex : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = queueCreateInfos.size();
        createInfo.pQueueCreateInfos = queueCreateInfos.data();


        createInfo.enabledExtensionCount =
            static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (validationLayerNames.size() > 0)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayerNames.size());
            createInfo.ppEnabledLayerNames = validationLayerNames.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        auto createResult = vkCreateDevice(
            physicalDevice, &createInfo, nullptr, &logicalDevice);
        if (createResult != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(logicalDevice, queueIndices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(logicalDevice, queueIndices.presentFamily.value(), 0, &presentQueue);
    }

}