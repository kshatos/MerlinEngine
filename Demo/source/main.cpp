#include <sstream>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include "Merlin/Core/core.hpp"
#include "Merlin/Render/render.hpp"
#include "Merlin/Scene/scene.hpp"

using namespace Merlin;


Vertex_XNUV verts[]
{
    // positions             // normal              // texture coords
    {-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f},
    { 0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f},
    {-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f},

    { 0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f},
    { 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f},
    { 0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f},

    { 0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f},
    {-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f},
    { 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f},

    {-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f},
    {-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f},
    {-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f},
    {-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f},

    {-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f},
    { 0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f},
    {-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f},

    { 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f},
    {-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f},
    { 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f},
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

std::shared_ptr<Camera> camera;
std::shared_ptr<VertexArray> main_varray;
std::shared_ptr<Texture2D> main_texture;
std::shared_ptr<Cubemap> main_cubemap;
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
        transform_comp = m_parent->GetComponent<TransformComponent>();
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

class FollowCameraComponent : public Component
{
    std::shared_ptr<TransformComponent> transform_comp;
public:
    FollowCameraComponent(Entity* parent) : Component(parent)
    {
    }

    virtual void OnAwake() override
    {
        transform_comp = m_parent->GetComponent<TransformComponent>();
    }

    virtual void OnUpdate(float time_step) override
    {
        transform_comp->transform = camera->GetTransform();
    }

};

class SceneLayer : public Layer
{
    GameScene scene;
    std::shared_ptr<FrameBuffer> fbuffer;
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

        main_cubemap = Cubemap::Create(
            std::vector<std::string>
            {
                ".\\Assets\\Textures\\skybox_hilly_lake\\right.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\left.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\top.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\bottom.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\front.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\back.jpg"
            });

        main_shader = Shader::CreateFromFiles(
            ".\\Assets\\Shaders\\basic_lit.vert",
            ".\\Assets\\Shaders\\basic_lit.frag");

        main_shader->Bind();
        main_shader->SetUniformInt("u_Texture", 0);

        Mesh<Vertex_XNUV> mesh;
        mesh.SetVertexData(verts, sizeof(verts) / sizeof(Vertex_XNUV));
        mesh.SetIndexData(tris, sizeof(tris) / sizeof(uint32_t));
        main_varray = UploadMesh(mesh);

        // Initialize camera
        camera = std::make_shared<PerspectiveCamera>(glm::pi<float>() / 2.0f, 1.0f, 0.1f, 20.0f);
        camera->GetTransform().Translate(glm::vec3(0.0f, 0.0f, 10.0f));
        scene.SetCamera(camera);

        // Add entities to the scene
        {
            auto entity = std::make_shared<Entity>();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto light_comp = entity->AddComponent<SpotLightComponent>();
            light_comp->data.color = glm::vec3(1.0f, 1.0f, 1.0f);
            light_comp->data.cutoff = glm::cos(glm::pi<float>() / 10);
            light_comp->data.intensity = 1.0f;
            light_comp->data.range = 2.0f;
            auto follow_cam_comp = entity->AddComponent<FollowCameraComponent>();

            scene.AddEntity(entity);
        }
        {
            auto entity = std::make_shared<Entity>();
            auto light_comp = entity->AddComponent<DirectionalLightComponent>();
            light_comp->data.color = glm::vec3(0.8, 0.8, 0.8);
            light_comp->data.direction = glm::vec3(0.2, 0.2, 0.2);

            scene.AddEntity(entity);
        }
        for (int i = 0; i < 4; ++i)
        {
            auto entity = std::make_shared<Entity>();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            transform_comp->transform.Translate(glm::vec3(
                glm::linearRand(-5.0f, 5.0f),
                glm::linearRand(-5.0f, 5.0f),
                glm::linearRand(-5.0f, 5.0f)));
            auto light_comp = entity->AddComponent<PointLightComponent>();
            light_comp->data.color = glm::vec3(1.0f, 1.0f, 1.0f);
            light_comp->data.intensity = 1.0f;
            light_comp->data.range = 2.0f;

            scene.AddEntity(entity);
        }

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

    virtual void OnAttach()override
    {
        FrameBufferParameters fb_params;
        fb_params.width = 800;
        fb_params.height = 800;
        fbuffer = FrameBuffer::Create(fb_params);
    }

    virtual void OnDetatch() override {}

    virtual void OnUpdate(float time_step) override
    {
        ME_LOG_INFO("fps: " + std::to_string(1.0f / time_step));

        scene.OnUpdate(time_step);
        MoveCamera(time_step);

        // Rendering
        {// Prepare background
            Renderer::SetViewport(
                0, 0,
                Application::Get().GeMaintWindow()->GetWidth(),
                Application::Get().GeMaintWindow()->GetHeight());
            Renderer::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
            Renderer::Clear();
        }

        {// Render scene to framebuffer
            fbuffer->Bind();
            Renderer::Clear();
            Renderer::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
            main_texture->Bind();
            scene.RenderScene();
            fbuffer->UnBind();
        }

        {// Render GUI to main window
            auto& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(
                (float)Application::Get().GeMaintWindow()->GetWidth(),
                (float)Application::Get().GeMaintWindow()->GetHeight());

            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            ImGui::Begin("Scene");
            uint32_t tex_id = fbuffer->GetColorAttachmentID();
            ImGui::Image((ImTextureID)tex_id, ImVec2{ 800, 800 });
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }

    virtual void HandleEvent(AppEvent& app_event) override
    {
        ME_LOG_INFO(app_event.ToString());
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
        {
            float th = Input::GetMouseScrollDelta().y * 1.0e-1f;
            float c = glm::cos(th);
            float s = glm::sin(th);
            camera->GetTransform().Rotate(glm::quat(c, 0.0f, s, 0.0f));
        }
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
