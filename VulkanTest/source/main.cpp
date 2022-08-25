#include <sstream>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include "Merlin/Core/core.hpp"
#include "Merlin/Render/render.hpp"
#include "Merlin/Scene/scene.hpp"
#include "Merlin/Core/file_util.hpp"

using namespace Merlin;


class ImGUITestLayer : public Layer
{
    std::shared_ptr<MeshBuffer> m_meshBuffer;
    std::shared_ptr<Texture2D> m_texture;

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
        BufferLayout layout{
            {ElementDataType::Float3, "Pos"}
        };

        m_meshBuffer = Renderer::CreateMeshBuffer(
            vertices.data(),
            vertices.size() * sizeof(float),
            indices.data(),
            indices.size(),
            layout);

        m_texture = Renderer::CreateTexture2D(
            LoadTexture(".\\Assets\\Textures\\debug.jpg"));
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
        RenderBackend::Vulkan,
        800,
        800
    };
    VulkanTestApp app(appInfo);
    app.Run();
}
