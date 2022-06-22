#include <sstream>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include "Merlin/Core/core.hpp"
#include "Merlin/Render/render.hpp"
#include "Merlin/Scene/scene.hpp"

using namespace Merlin;



class VulkanTestApp : public Application
{
public:
    VulkanTestApp(const ApplicationInfo& properties) : Application(properties)
    {
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
