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
#include "Merlin/Scene/scene.hpp"
#include <glm/gtc/random.hpp>

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

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
out vec2 TexCoord;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
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

std::shared_ptr<VertexArray> main_varray;
std::shared_ptr<Texture2D> main_texture;
std::shared_ptr<Shader> main_shader;

class SpinningComponent : public Component
{
    glm::vec3 axis;
    float rotation_speed;
    std::shared_ptr<TransformComponent> transform_comp;

public:
    SpinningComponent(Entity* parent) : Component(parent)
    {
        rotation_speed = glm::linearRand(0.05f, 0.50f);
        axis = glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f));
        axis = glm::normalize(axis);
    }

    virtual void OnAwake() override
    {
        transform_comp = parent->GetComponent<TransformComponent>();
    }

    virtual void OnUpdate(float time_step) override
    {
        auto th = time_step * rotation_speed;
        auto c = glm::cos(th);
        auto s = glm::sin(th);

        glm::quat q(c, s * axis.x, s * axis.y, s * axis.z);
        transform_comp->transform.Rotate(q);
    }

};

class SceneLayer : public Layer
{
    Scene scene;
    std::shared_ptr<Camera> camera;
public:
    SceneLayer()
    {
        // Initialize render data
        main_texture = Texture2D::Create(
            ".\\Assets\\Textures\\debug.jpg",
            Texture2DProperties(
                TextureWrapMode::Repeat,
                TextureWrapMode::Repeat,
                TextureFilterMode::Linear));

        main_shader = std::shared_ptr<Shader>(Shader::Create(vertex_source, fragment_source));
        main_shader->Bind();
        main_shader->SetUniformInt("u_Texture", 0);

        BufferLayout layout{
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float2, "aTexCoord"},
        };

        auto vbuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(verts, sizeof(verts)));
        vbuffer->SetLayout(layout);

        auto ibuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(tris, sizeof(tris) / sizeof(uint32_t)));

        main_varray = std::shared_ptr<VertexArray>(VertexArray::Create());
        main_varray->AddVertexBuffer(vbuffer);
        main_varray->SetIndexBuffer(ibuffer);

        // Initialize camera
        camera = std::make_shared<PerspectiveCamera>(glm::pi<float>() / 2.0f, 1.0f, 0.1f, 20.0f);
        camera->GetTransform().Translate(glm::vec3(0.0f, 0.5f, 2.0f));
        scene.SetCamera(camera);

        // Add entities to the scene
        for (int i = 0; i < 600; ++i)
        {
            auto entity = std::make_shared<Entity>();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto mesh_comp = entity->AddComponent<MeshRenderComponent>();
            auto spin_comp = entity->AddComponent<SpinningComponent>();

            transform_comp->transform.Translate(glm::vec3(
                glm::linearRand(-5.0f, 5.0f),
                glm::linearRand(-5.0f, 5.0f),
                glm::linearRand(-5.0f, 5.0f)));
            transform_comp->transform.Scale(
                glm::vec3(glm::linearRand(0.3f, 0.8f)));
            mesh_comp->shader = main_shader;
            mesh_comp->varray = main_varray;
            scene.AddEntity(entity);
        }

        scene.OnAwake();
    }

    virtual void OnAttach()override {}

    virtual void OnDetatch() override {}

    virtual void OnUpdate(float time_step) override
    {
        scene.OnUpdate(time_step);
        MoveCamera(time_step);

        ME_LOG_INFO("fps: " + std::to_string(1.0f / time_step));
        Renderer::SetViewport(
            0, 0,
            Application::Get().GeMaintWindow()->GetWidth(),
            Application::Get().GeMaintWindow()->GetHeight());
        Renderer::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        Renderer::Clear();
        scene.RenderScene();

        // GUI
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

    void MoveCamera(float time_step)
    {
        float speed = 5.0e-1f;
        if (Input::GetKeyDown(Key::W))
            camera->GetTransform().Translate(glm::vec3(0.0f, 0.0f, -speed * time_step));
        if (Input::GetKeyDown(Key::A))
            camera->GetTransform().Translate(glm::vec3(-speed * time_step, 0.0f, 0.0f));
        if (Input::GetKeyDown(Key::S))
            camera->GetTransform().Translate(glm::vec3(0.0f, 0.0f, speed * time_step));
        if (Input::GetKeyDown(Key::D))
            camera->GetTransform().Translate(glm::vec3(speed * time_step, 0.0f, 0.0f));
        if (Input::GetKeyDown(Key::Z))
            camera->GetTransform().Translate(glm::vec3(0.0f, speed * time_step, 0.0f));
        if (Input::GetKeyDown(Key::X))
            camera->GetTransform().Translate(glm::vec3(0.0f, -speed * time_step, 0.0f));
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
