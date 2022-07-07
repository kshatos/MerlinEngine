#include <sstream>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include "Merlin/Core/core.hpp"
#include "Merlin/Render/render.hpp"
#include "Merlin/Scene/scene.hpp"

using namespace Merlin;


class ImGUITestLayer : public Layer
{
    virtual void OnAttach() {};

    virtual void OnDetatch() {};

    virtual void OnUpdate(float time_step)
    {
        ImGui::ShowDemoWindow();
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
