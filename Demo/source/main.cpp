#include <sstream>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include "Merlin/Core/core.hpp"
#include "Merlin/Render/render.hpp"
#include "Merlin/Scene/scene.hpp"
#include "uv_sphere.hpp"

using namespace Merlin;


Vertex_XNUV CubeVerts[]
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

uint32_t CubeIndices[]
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
std::shared_ptr<VertexArray> cube_varray;
std::shared_ptr<VertexArray> sphere_varray;
std::shared_ptr<Cubemap> main_cubemap;
std::shared_ptr<Shader> skybox_shader;
std::shared_ptr<Material> main_material;
std::shared_ptr<Material> pbr_texture_material;

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
    float m_ambientRadiance = 0.0f;
    GameScene scene;
    std::shared_ptr<FrameBuffer> fbuffer;
public:
    SceneLayer()
    {
        // Initialize render data
        auto texProps = Texture2DProperties(
            TextureWrapMode::Repeat,
            TextureWrapMode::Repeat,
            TextureFilterMode::Linear);
        auto main_texture = Texture2D::Create(
            ".\\Assets\\Textures\\debug.jpg", texProps);
        auto pbr_albedo_texture = Texture2D::Create(
            ".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Color.jpg", texProps);
        auto pbr_roughness_texture = Texture2D::Create(
            ".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Roughness.jpg", texProps);
        auto pbr_metalic_texture = Texture2D::Create(
            ".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Metalness.jpg", texProps);
        auto pbr_normal_texture = Texture2D::Create(
            ".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_NormalGL.jpg", texProps);

        auto cube_data = std::make_shared<CubemapData>(100, 3);
        for (int face_id = CubeFace::Begin; face_id < CubeFace::End; ++face_id)
        {
            auto face = static_cast<CubeFace>(face_id);
            for (int j = 0; j < 100; ++j)
            {
                for (int i = 0; i < 100; ++i)
                {
                    auto point = cube_data->GetPixelCubePoint(face, i, j);
                    point = glm::normalize(point);
                    point *= 0.5f;
                    point += 0.5f;
                    cube_data->GetPixel(face, i, j, 0) = point.x;
                    cube_data->GetPixel(face, i, j, 1) = point.y;
                    cube_data->GetPixel(face, i, j, 2) = point.z;
                }
            }
        }
        auto custom_cubemap = UploadCubemap(cube_data);

        main_cubemap = Cubemap::Create(
            std::vector<std::string>
        {
            ".\\Assets\\Textures\\skybox_hilly_lake\\right.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\left.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\bottom.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\top.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\front.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\back.jpg"
        });

        auto pbr_shader = Shader::CreateFromFiles(
            ".\\Assets\\Shaders\\pbr_lit_basic.vert",
            ".\\Assets\\Shaders\\pbr_lit_basic.frag");

        auto pbr_texture_shader = Shader::CreateFromFiles(
            ".\\Assets\\Shaders\\pbr_lit_texture.vert",
            ".\\Assets\\Shaders\\pbr_lit_texture.frag");

        auto main_shader = Shader::CreateFromFiles(
            ".\\Assets\\Shaders\\basic_lit.vert",
            ".\\Assets\\Shaders\\basic_lit.frag");

        skybox_shader = Shader::CreateFromFiles(
            ".\\Assets\\Shaders\\skybox.vert",
            ".\\Assets\\Shaders\\skybox.frag");

        pbr_texture_material = std::make_shared<Material>(
            pbr_texture_shader,
            BufferLayout{},
            std::vector<std::string>{
                "u_albedoTexture",
                "u_roughnessTexture",
                "u_metalicTexture",
                "u_normalTexture"
            }
        );
        pbr_texture_material->SetTexture("u_albedoTexture", pbr_albedo_texture);
        pbr_texture_material->SetTexture("u_roughnessTexture", pbr_roughness_texture);
        pbr_texture_material->SetTexture("u_metalicTexture", pbr_metalic_texture);
        pbr_texture_material->SetTexture("u_normalTexture", pbr_normal_texture);

        main_material = std::make_shared<Material>(
            pbr_shader,
            BufferLayout{
                {ShaderDataType::Float3, "u_albedo"},
                {ShaderDataType::Float, "u_roughness"},
                {ShaderDataType::Float, "u_metalic"}
            },
            std::vector<std::string>{});
        main_material->SetUniformFloat3("u_albedo", glm::vec3(0.0, 0.0, 0.0));
        main_material->SetUniformFloat("u_roughness", 0.0f);
        main_material->SetUniformFloat("u_metalic", 0.0f);

        auto cubeMesh = std::make_shared<Mesh<Vertex_XNUV>>();
        cubeMesh->SetVertexData(CubeVerts, sizeof(CubeVerts) / sizeof(Vertex_XNUV));
        cubeMesh->SetIndexData(CubeIndices, sizeof(CubeIndices) / sizeof(uint32_t));
        cube_varray = UploadMesh(cubeMesh);

        auto sphereMesh =  std::make_shared<Mesh<Vertex_XNTBUV>>();
        sphereMesh->SetVertexData(UVSphereVerts, sizeof(UVSphereVerts) / sizeof(Vertex_XNTBUV));
        sphereMesh->SetIndexData(UVSphereIndices, sizeof(UVSphereIndices) / sizeof(uint32_t));
        CalculateTangentFrame(sphereMesh);
        sphere_varray = UploadMesh(sphereMesh);

        //auto skybox = std::make_shared<Skybox>(main_cubemap, 15.0);
        //skybox->SetShader(skybox_shader);
        //scene.SetSkybox(skybox);

        // Initialize camera
        camera = std::make_shared<PerspectiveCamera>(glm::pi<float>() / 2.0f, 1.0f, 0.1f, 30.0f);
        camera->GetTransform().Translate(glm::vec3(0.0f, 0.0f, 5.0f));
        scene.SetCamera(camera);

        // Add entities to the scene
        {
            auto entity = std::make_shared<Entity>();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto light_comp = entity->AddComponent<SpotLightComponent>();
            light_comp->data.color = glm::vec3(1.0f, 1.0f, 1.0f);
            light_comp->data.cutoffAngle = glm::pi<float>() / 10;
            light_comp->data.falloffRatio = 0.25f;
            light_comp->data.radiantIntensity = 10.0f;
            light_comp->data.range = 50.0f;
            auto follow_cam_comp = entity->AddComponent<FollowCameraComponent>();

            scene.AddEntity(entity);
        }
        {
            auto entity = std::make_shared<Entity>();
            auto light_comp = entity->AddComponent<DirectionalLightComponent>();
            light_comp->data.color = glm::vec3(0.2, 0.2, 1.0);
            light_comp->data.irradiance = 5.0f;
            light_comp->data.direction = glm::vec3(0.0, 1.0, 0.0);

            scene.AddEntity(entity);
        }
        for (int i = 0; i < 4; ++i)
        {
            auto entity = std::make_shared<Entity>();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            transform_comp->transform.Translate(glm::vec3(
                glm::linearRand(-2.0f, 2.0f),
                glm::linearRand(-2.0f, 2.0f),
                5.0f));
            auto light_comp = entity->AddComponent<PointLightComponent>();
            light_comp->data.color = glm::vec3(1.0f, 1.0f, 1.0f);
            light_comp->data.radiantFlux = 50.0f;
            light_comp->data.range = 50.0f;

            scene.AddEntity(entity);
        }

        {
            auto entity = std::make_shared<Entity>();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto mesh_comp = entity->AddComponent<MeshRenderComponent>();
            //auto spin_comp = entity->AddComponent<SpinningComponent>();

            transform_comp->transform.Rotate(glm::vec3(-1.0, 0.0, 0.0), 0.5f * glm::pi<float>());

            mesh_comp->varray = sphere_varray;
            mesh_comp->material = pbr_texture_material;

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
            Renderer::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
            Renderer::Clear();
        }

        {// Render scene to framebuffer
            fbuffer->Bind();
            Renderer::Clear();
            Renderer::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
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

            // Scene viewport
            ImGui::Begin("Scene");
            uint32_t tex_id = fbuffer->GetColorAttachmentID();
            ImGui::Image((ImTextureID)tex_id, ImVec2{ 900, 900 });
            ImGui::End();

            // Prompt
            ImGui::Begin("Settings");
            ImGui::SliderFloat("Ambient Light", &m_ambientRadiance, 0.0f, 1.0f);
            Renderer::SetAmbientLighting(m_ambientRadiance);
            ImGui::End();

            // Finish
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
        const auto& up = camera->GetTransform().Up();
        const auto& right = camera->GetTransform().Right();
        const auto& forward = camera->GetTransform().Forward();
        float speed = 5.0e-1f;
        if (Input::GetKeyDown(Key::LEFT_SHIFT))
            speed *= 2.0f;
        if (Input::GetKeyDown(Key::W))
            camera->GetTransform().Translate(+forward * speed * time_step);
        if (Input::GetKeyDown(Key::A))
            camera->GetTransform().Translate(-right * speed * time_step);
        if (Input::GetKeyDown(Key::S))
            camera->GetTransform().Translate(-forward * speed * time_step);
        if (Input::GetKeyDown(Key::D))
            camera->GetTransform().Translate(+right * speed * time_step);
        if (Input::GetKeyDown(Key::Z))
            camera->GetTransform().Translate(+up * speed * time_step);
        if (Input::GetKeyDown(Key::X))
            camera->GetTransform().Translate(-up * speed * time_step);

        camera->GetTransform().Rotate(up, Input::GetMouseScrollDelta().y * time_step * 5.0);
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
