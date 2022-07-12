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
    std::shared_ptr<VertexBuffer> m_vertexBuffer;

public:
    virtual void OnAttach() override
    {
        std::vector<float> vertices{
            1.0, 2.0, 3.0, 4.0, 5.0
        };

        m_vertexBuffer = Renderer::CreateVertexBuffer(
            vertices.data(), vertices.size());
    };

    virtual void OnDetatch() override {};

    virtual void OnUpdate(float time_step) override
    {
        ImGui::ShowDemoWindow();
    }

    virtual void HandleEvent(AppEvent& app_event) override {};
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
