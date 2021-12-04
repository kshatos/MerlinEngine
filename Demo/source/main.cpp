#include <sstream>
#include "Merlin/Core/logger.hpp"
#include "Merlin/Core/window.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/buffer_layout.hpp"
#include "Merlin/Render/vertex_array.hpp"
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
    -0.5f, -0.5f, +0.5f, 0.0f, 0.0f , 1.0f, 1.0f,
    +0.5f, -0.5f, +0.5f, 1.0f, 0.0f , 1.0f, 1.0f,
    +0.5f, +0.5f, +0.5f, 1.0f, 1.0f , 1.0f, 1.0f,
    -0.5f, +0.5f, +0.5f, 0.0f, 1.0f , 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f , 0.0f, 1.0f,
    +0.5f, -0.5f, -0.5f, 1.0f, 0.0f , 0.0f, 1.0f,
    +0.5f, +0.5f, -0.5f, 1.0f, 1.0f , 0.0f, 1.0f,
    -0.5f, +0.5f, -0.5f, 0.0f, 1.0f , 0.0f, 1.0f
};

uint32_t tris[]
{
    0, 1, 2,
    0, 2, 3,
    5, 4, 7,
    5, 7, 6,
    1, 5, 6,
    1, 6, 2,
    4, 0, 3,
    4, 3, 7,
    3, 2, 6,
    3, 6, 7,
    4, 5, 1,
    4, 1, 0,
};

auto vertex_source =
R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

out vec4 color;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;


void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aCol;
}
)";

auto fragment_source =
R"(
#version 330 core
out vec4 FragColor;
in vec4 color;


void main()
{
    FragColor = color;
}
)";


class DemoApplication : public Application
{
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<VertexArray> varray;
public:
    DemoApplication() : Application()
    {
        camera = std::make_shared<PerspectiveCamera>(glm::pi<float>() / 2.0f, 1.0f, 0.01f, 10.0f);
        camera->GetTransform().Translate(glm::vec3(0.0f, 0.5f, 2.0f));

        shader = std::shared_ptr<Shader>(Shader::Create(vertex_source, fragment_source));

        BufferLayout layout{
            {ShaderDataType::Float3, "pos"},
            {ShaderDataType::Float4, "color"},
        };

        auto vbuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(verts, sizeof(verts)));
        vbuffer->SetLayout(layout);

        auto ibuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(tris, sizeof(tris) / sizeof(uint32_t)));

        varray = std::shared_ptr<VertexArray>(VertexArray::Create());
        varray->AddVertexBuffer(vbuffer);
        varray->SetIndexBuffer(ibuffer);
    }

    virtual void HandleEvent(AppEvent& app_event) override
    {
        //ME_LOG_INFO(app_event.ToString());

        Application::HandleEvent(app_event);

        AppEvent::Dispatch<KeyPressedEvent>(app_event,
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

        AppEvent::Dispatch<MouseScrolledEvent>(app_event,
            [this](MouseScrolledEvent& e)
        {
            float th = e.GetYScroll() * 1.0e-1f;
            float c = glm::cos(th);
            float s = glm::sin(th);
            camera->GetTransform().Rotate(glm::quat(c, 0.0f, s, 0.0f));
            return true;
        });
    }

    virtual void OnUpdate() override
    {
        {
            auto pos = Input::GetMousePosition();
            std::ostringstream oss;
            oss << "Mouse Pos: (" << pos.x << ", " << pos.y << ")";
            oss << " Left: " << Input::GetMouseButtonDown(MouseButton::BUTTON_1);
            oss << " Right: " << Input::GetMouseButtonDown(MouseButton::BUTTON_2);
            oss << " Middle: " << Input::GetMouseButtonDown(MouseButton::BUTTON_3);
            oss << " A: " << Input::GetKeyDown(Key::A);
            ME_LOG_INFO(oss.str());
        }

        Renderer::SetViewport(0, 0, GeMaintWindow()->GetWidth(), GeMaintWindow()->GetHeight());
        Renderer::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        Renderer::Clear();

        // SCENE RENDER
        {
            Renderer::BeginScene(camera);
            Renderer::Submit(shader, varray);
            Renderer::EndScene();
        }

        // GUI RENDER
        {
            auto& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(
                (float)GeMaintWindow()->GetWidth(),
                (float)GeMaintWindow()->GetHeight());

            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        }
    }
};


void main()
{
    DemoApplication app;
    app.Run();
}
