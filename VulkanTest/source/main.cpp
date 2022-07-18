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
    std::shared_ptr<IndexBuffer> m_indexBuffer;

public:
    virtual void OnAttach() override
    {
        std::vector<float> vertices{
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0
        };

        std::vector<uint32_t> indices{
            0, 1, 2
        };

        m_vertexBuffer = Renderer::CreateVertexBuffer(
            vertices.data(), vertices.size());
        m_indexBuffer = Renderer::CreateIndexBuffer(
            indices.data(), indices.size());
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
