#include <sstream>
#include "Merlin/Core/logger.hpp"
#include "Merlin/Core/window.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/buffer_layout.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/texture2d.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/renderer.hpp"
#include "Merlin/Render/perspective_camera.hpp"
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include "Merlin/Core/application.hpp"
#include "Merlin/Core/input.hpp"

using namespace Merlin;

bool is_running = true;

float verts[]
{
     // positions          // texture coords
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
                          
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
                          
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

     0.5f,  -0.5f,  0.5f,   0.0f, 0.0f,
    -0.5f,  -0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f,  -0.5f, -0.5f,   1.0f, 1.0f,
     0.5f,  -0.5f, -0.5f,   0.0f, 1.0f,
};

uint32_t tris[]
{
    0, 1, 2,
    0, 2, 3,

    4, 5, 6,
    4, 6, 7,

    8, 9, 10,
    8, 10, 11,

    12, 13, 14,
    12, 14, 15,

    16, 17, 18,
    16, 18, 19,

    20, 21, 22,
    20, 22, 23
};

auto vertex_source =
R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
out vec2 TexCoord;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)";

auto fragment_source =
R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, TexCoord);
}
)";


class SceneLayer : public Layer
{
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<VertexArray> varray;
    std::shared_ptr<Texture2D> texture;
public:
    SceneLayer()
    {
        camera = std::make_shared<PerspectiveCamera>(glm::pi<float>() / 2.0f, 1.0f, 0.01f, 10.0f);
        camera->GetTransform().Translate(glm::vec3(0.0f, 0.5f, 2.0f));

        texture = Texture2D::Create(
            "C:\\Users\\kshat\\Desktop\\debug.jpg",
            Texture2DProperties(
                TextureWrapMode::Repeat,
                TextureWrapMode::Repeat,
                TextureFilterMode::Linear));

        shader = std::shared_ptr<Shader>(Shader::Create(vertex_source, fragment_source));
        shader->Bind();
        shader->SetUniformInt("u_Texture", 0);

        BufferLayout layout{
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float2, "aTexCoord"},
        };

        auto vbuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(verts, sizeof(verts)));
        vbuffer->SetLayout(layout);

        auto ibuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(tris, sizeof(tris) / sizeof(uint32_t)));

        varray = std::shared_ptr<VertexArray>(VertexArray::Create());
        varray->AddVertexBuffer(vbuffer);
        varray->SetIndexBuffer(ibuffer);
    }

    virtual void OnAttach()override {}
    
    virtual void OnDetatch() override {}
    
    virtual void OnUpdate() override
    {
        Renderer::SetViewport(
            0, 0,
            Application::Get().GeMaintWindow()->GetWidth(),
            Application::Get().GeMaintWindow()->GetHeight());
        Renderer::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        Renderer::Clear();

        // SCENE RENDER
        {
            texture->Bind();
            Renderer::BeginScene(camera);
            Renderer::Submit(shader, varray);
            Renderer::EndScene();
        }

        // GUI RENDER
        {
            auto& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(
                (float)Application::Get().GeMaintWindow()->GetWidth(),
                (float)Application::Get().GeMaintWindow()->GetHeight());

            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        }
    }

    virtual void HandleEvent(AppEvent& app_event) override
    {
        ME_LOG_INFO(app_event.ToString());

        app_event.Dispatch<KeyPressedEvent>(
            [this](KeyPressedEvent& e)
        {
            float speed = 1.0e-1f;
            if (e.GetKeyCode() == Key::W)
                camera->GetTransform().Translate(glm::vec3(0.0f, 0.0f, -speed));
            if (e.GetKeyCode() == Key::A)
                camera->GetTransform().Translate(glm::vec3(-speed, 0.0f, 0.0f));
            if (e.GetKeyCode() == Key::S)
                camera->GetTransform().Translate(glm::vec3(0.0f, 0.0f, speed));
            if (e.GetKeyCode() == Key::D)
                camera->GetTransform().Translate(glm::vec3(speed, 0.0f, 0.0f));
            if (e.GetKeyCode() == Key::Z)
                camera->GetTransform().Translate(glm::vec3(0.0f, speed, 0.0f));
            if (e.GetKeyCode() == Key::X)
                camera->GetTransform().Translate(glm::vec3(0.0f, -speed, 0.0f));
            return true;
        });

        app_event.Dispatch<MouseScrolledEvent>(
            [this](MouseScrolledEvent& e)
        {
            float th = e.GetYScroll() * 1.0e-1f;
            float c = glm::cos(th);
            float s = glm::sin(th);
            camera->GetTransform().Rotate(glm::quat(c, 0.0f, s, 0.0f));
            return true;
        });
    }

};

class DemoApplication : public Application
{
public:
    DemoApplication() : Application()
    {
        PushLayerFront(std::make_shared<SceneLayer>());
    }
};


void main()
{
    DemoApplication app;
    app.Run();
}
