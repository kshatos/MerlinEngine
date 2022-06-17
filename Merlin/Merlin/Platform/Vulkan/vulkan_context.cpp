#include <stdexcept>
#include <vector>
#include "vulkan_context.hpp"



namespace Merlin
{
    VkApplicationInfo CreateAppInfo()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Merlin Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        return appInfo;
    }

    void GetRequiredInstanceExtensions(
        VkInstanceCreateInfo& createInfo)
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
    }


    QueueFamilyIndices GetQueueFamilies(
        const VkPhysicalDevice& physicalDevice)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
                break;
            }
            i++;
        }

        return indices;
    }

    void VulkanContext::CreateInstance()
    {
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &CreateAppInfo();
        GetRequiredInstanceExtensions(createInfo);

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
            throw std::runtime_error("");
    }

    void VulkanContext::CreateSurface(GLFWwindow* window)
    {
        auto surfaceCreateStatus = glfwCreateWindowSurface(
            m_instance, window, nullptr, &m_surface);

        if (surfaceCreateStatus != VK_SUCCESS)
            throw std::runtime_error("failed to create window surface!");
    }

    void VulkanContext::PickPhysicalDevice()
    {
        m_physicalDevice = VK_NULL_HANDLE;

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        if (deviceCount == 0)
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
        for (const auto& device : devices)
        {
            if (deviceIsSuitable(device))
            {
                m_physicalDevice = device;
                break;
            }
        }

        if (m_physicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("failed to find a suitable GPU!");
    }

    bool deviceIsSuitable(const VkPhysicalDevice& device)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        QueueFamilyIndices indices = GetQueueFamilies(device);

        return indices.graphicsFamily.has_value();
    }

    VkDeviceQueueCreateInfo VulkanContext::GetDeviceQueueInfo()
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = m_indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        return queueCreateInfo;
    }

    void VulkanContext::CreateLogicalDevice()
    {
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(m_physicalDevice, &deviceFeatures);

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = &GetDeviceQueueInfo();
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = 0;
        deviceCreateInfo.enabledLayerCount = 0;

        auto deviceCreateStatus = vkCreateDevice(
            m_physicalDevice,
            &deviceCreateInfo,
            nullptr,
            &m_logicalDevice);

        if (deviceCreateStatus != VK_SUCCESS)
            throw std::runtime_error("failed to create logical device!");
    }


    VulkanContext::VulkanContext(GLFWwindow* window)
    {
        CreateInstance();
        CreateSurface(window);
        PickPhysicalDevice();
        m_indices = GetQueueFamilies(m_physicalDevice);
        CreateLogicalDevice();
        vkGetDeviceQueue(
            m_logicalDevice,
            m_indices.graphicsFamily.value(), 
            0,
            &m_graphicsQueue);
    }

    VulkanContext::~VulkanContext()
    {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyInstance(m_instance, nullptr);
        vkDestroyDevice(m_logicalDevice, nullptr);
    }

    void VulkanContext::Init()
    {
    }

    void VulkanContext::SwapBuffers()
    {
    }

}
