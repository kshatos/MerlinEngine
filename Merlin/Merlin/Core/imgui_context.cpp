#include "imgui_context.hpp"
#ifdef MERLIN_USE_OPENGL
#include <backends/imgui_impl_opengl3.h>
#endif
#ifdef MERLIN_USE_VULKAN
#include <backends/imgui_impl_vulkan.h>
#include "Merlin/Platform/Vulkan/vulkan_render_api.hpp"
#endif
#include "backends/imgui_impl_glfw.h"


namespace Merlin
{
    ImGuiAPI::~ImGuiAPI()
    {
        if (!initialized)
            return;

        switch (backend)
        {
        case RenderBackend::OPENGL:
#ifdef MERLIN_USE_OPENGL
            ImGui_ImplOpenGL3_Shutdown();
#endif
            break;
        case RenderBackend::VULKAN:
#ifdef MERLIN_USE_VULKAN
            ImGui_ImplVulkan_Shutdown();
#endif
            break;
        }

        ImGui::DestroyContext(context);
    }

    void ImGuiAPI::Init(void* window, std::shared_ptr<RenderAPI> renderApi)
    {
        backend = renderApi->Backend();

        IMGUI_CHECKVERSION();
        context = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        InitImGuiImpl(window, renderApi);
        initialized = true;
    }

    void ImGuiAPI::InitImGuiImpl(void* window, std::shared_ptr<RenderAPI> renderApi)
    {
        switch (renderApi->Backend())
        {
        case RenderBackend::OPENGL:
#ifdef MERLIN_USE_OPENGL
            ImGui_ImplOpenGL3_Init("#version 330 core");
#else
            throw std::runtime_error("OpenGL not usable.");
#endif
            break;
        case RenderBackend::VULKAN:
#ifdef MERLIN_USE_VULKAN
            auto vulkanApi = std::dynamic_pointer_cast <VulkanRenderAPI>(renderApi);

            ImGui_ImplVulkan_InitInfo init_info = {};
            init_info.Instance = vulkanApi->instance;
            init_info.PhysicalDevice = vulkanApi->physicalDevice;
            init_info.Device = vulkanApi->logicalDevice;
            /*
            init_info.QueueFamily = g_QueueFamily;
            init_info.Queue = g_Queue;
            init_info.PipelineCache = g_PipelineCache;
            init_info.DescriptorPool = g_DescriptorPool;
            init_info.Allocator = g_Allocator;
            init_info.MinImageCount = g_MinImageCount;
            init_info.ImageCount = wd->ImageCount;
            init_info.CheckVkResultFn = check_vk_result;

            ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
            */

#else
            throw std::runtime_error("Vulkan not usable.");
#endif
            break;
        }
    }

    void ImGuiAPI::HandleEvent(AppEvent& app_event)
    {
        app_event.Dispatch<MouseButtonPressedEvent>(
            [](MouseButtonPressedEvent& e)
            {
                auto& io = ImGui::GetIO();
                io.MouseDown[e.GetButton()] = true;
                return false;
            });
        app_event.Dispatch<MouseButtonReleasedEvent>(
            [](MouseButtonReleasedEvent& e)
            {
                auto& io = ImGui::GetIO();
                io.MouseDown[e.GetButton()] = false;
                return false;
            });
        app_event.Dispatch<MouseMovedEvent>(
            [](MouseMovedEvent& e)
            {
                auto& io = ImGui::GetIO();
                io.MousePos = ImVec2(e.GetXPosition(), e.GetYPosition());
                return false;
            });
        app_event.Dispatch<MouseScrolledEvent>(
            [](MouseScrolledEvent& e)
            {
                auto& io = ImGui::GetIO();
                io.MouseWheel += e.GetYScrollDelta();
                io.MouseWheelH += e.GetXScrollDelta();
                return false;
            });
    }
}