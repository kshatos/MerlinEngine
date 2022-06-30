#include <sstream>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include "Merlin/Core/core.hpp"
#include "Merlin/Render/render.hpp"
#include "Merlin/Scene/scene.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
using namespace Merlin;


class ImGUITestLayer : public Layer
{
    virtual void OnAttach() {};

    virtual void OnDetatch() {};

    virtual void OnUpdate(float time_step)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        /*
        */
    }

    virtual void HandleEvent(AppEvent& app_event) {};
};


class VulkanTestApp : public Application
{
public:
    VulkanTestApp(const ApplicationInfo& properties) : Application(properties)
    {
        PushLayerBack(std::make_shared<ImGUITestLayer>());
    }
};


void main()
{
    ApplicationInfo appInfo
    {
        "VulkanTest",
        RenderBackend::VULKAN,
        800,
        800
    };
    VulkanTestApp app(appInfo);
    app.Run();
}
