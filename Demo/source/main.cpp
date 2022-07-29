#include <sstream>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include"Merlin/Core/file_util.hpp"
#include "Merlin/Core/core.hpp"
#include "Merlin/Render/render.hpp"
#include "Merlin/Scene/scene.hpp"
#include "uv_sphere.hpp"

using namespace Merlin;


Vertex_XNTBUV CubeVerts[]
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

std::shared_ptr<TransformComponent> camera_transform;
std::shared_ptr<MeshBuffer> cube_mbuffer;
std::shared_ptr<MeshBuffer> sphere_mbuffer;
std::shared_ptr<Cubemap> main_cubemap;
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
        transform_comp->transform = camera_transform->transform;
    }

};

class SceneLayer : public Layer
{
    float m_ambientRadiance = 0.0f;
    GameScene scene;
    CameraRenderData* camera_data;
    ImVec2 viewport_size{ 0, 0 };

public:
    SceneLayer()
    {
        auto texProps = Texture2DProperties(
            TextureWrapMode::Repeat,
            TextureWrapMode::Repeat,
            TextureFilterMode::Linear);

        auto main_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\debug.jpg"), texProps);

        auto metal_plate_albedo_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Color.jpg"), texProps);
        auto metal_plate_roughness_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Roughness.jpg"), texProps);
        auto metal_plate_metalic_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Metalness.jpg"), texProps);
        auto metal_plate_normal_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_NormalGL.jpg"), texProps);

        auto helmet_albedo_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet_BaseColor.png"), texProps);
        auto helmet_roughness_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet_MetallicRoughness.png"), texProps);
        auto helmet_metalic_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet_MetallicRoughness.png"), texProps);
        auto helmet_normal_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet_Normal.png"), texProps);

        auto spitfire_albedo_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Models\\supermarine-spitfire\\spitfire_d.png"), texProps);
        auto spitfire_roughness_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Models\\supermarine-spitfire\\spitfire_r.png"), texProps);
        auto spitfire_metalic_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Models\\supermarine-spitfire\\spitfire_m.png"), texProps);
        auto spitfire_normal_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Models\\supermarine-spitfire\\spitfire_n.png"), texProps);

        main_cubemap = Renderer::CreateCubemap(
            std::vector<std::string>
        {
            ".\\Assets\\Textures\\skybox_hilly_lake\\right.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\left.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\top.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\bottom.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\front.jpg",
                ".\\Assets\\Textures\\skybox_hilly_lake\\back.jpg"
        });

        auto pbr_shader = Renderer::CreateShader(
            ".\\Assets\\Shaders\\pbr_lit_basic.vert",
            ".\\Assets\\Shaders\\pbr_lit_basic.frag");

        auto pbr_texture_shader = Renderer::CreateShader(
            ".\\Assets\\Shaders\\pbr_lit_texture.vert",
            ".\\Assets\\Shaders\\pbr_lit_texture.frag");

        auto main_shader = Renderer::CreateShader(
            ".\\Assets\\Shaders\\basic_lit.vert",
            ".\\Assets\\Shaders\\basic_lit.frag");

        pbr_texture_material = Renderer::CreateMaterial(
            MaterialInfo
            {
                pbr_texture_shader,
                BufferLayout{},
                std::vector<std::string>{
                "u_albedoTexture",
                    "u_roughnessTexture",
                    "u_metalicTexture",
                    "u_normalTexture"
                }
            }
        );
        auto metal_plate_material_instance = Renderer::CreateMaterialInstance(pbr_texture_material);
        metal_plate_material_instance->SetTexture("u_albedoTexture", metal_plate_albedo_texture);
        metal_plate_material_instance->SetTexture("u_roughnessTexture", metal_plate_roughness_texture);
        metal_plate_material_instance->SetTexture("u_metalicTexture", metal_plate_metalic_texture);
        metal_plate_material_instance->SetTexture("u_normalTexture", metal_plate_normal_texture);
        
        auto helmet_material_instance = Renderer::CreateMaterialInstance(pbr_texture_material);
        helmet_material_instance->SetTexture("u_albedoTexture", helmet_albedo_texture);
        helmet_material_instance->SetTexture("u_roughnessTexture", helmet_roughness_texture);
        helmet_material_instance->SetTexture("u_metalicTexture", helmet_metalic_texture);
        helmet_material_instance->SetTexture("u_normalTexture", helmet_normal_texture);

        auto spitfire_material_instance = Renderer::CreateMaterialInstance(pbr_texture_material);
        spitfire_material_instance->SetTexture("u_albedoTexture", spitfire_albedo_texture);
        spitfire_material_instance->SetTexture("u_roughnessTexture", spitfire_roughness_texture);
        spitfire_material_instance->SetTexture("u_metalicTexture", spitfire_metalic_texture);
        spitfire_material_instance->SetTexture("u_normalTexture", spitfire_normal_texture);

        auto cubeMesh = std::make_shared<Mesh<Vertex_XNTBUV>>();
        cubeMesh->SetVertexData(CubeVerts, sizeof(CubeVerts) / sizeof(Vertex_XNUV));
        cubeMesh->SetIndexData(CubeIndices, sizeof(CubeIndices) / sizeof(uint32_t));
        CalculateTangentFrame(cubeMesh);
        cube_mbuffer = Renderer::CreateMeshBuffer(
            cubeMesh->GetVertexDataPointer(),
            cubeMesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            cubeMesh->GetIndexDataPointer(),
            cubeMesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        auto sphereMesh = std::make_shared<Mesh<Vertex_XNTBUV>>();
        sphereMesh->SetVertexData(UVSphereVerts, sizeof(UVSphereVerts) / sizeof(Vertex_XNTBUV));
        sphereMesh->SetIndexData(UVSphereIndices, sizeof(UVSphereIndices) / sizeof(uint32_t));
        CalculateTangentFrame(sphereMesh);
        sphere_mbuffer = Renderer::CreateMeshBuffer(
            sphereMesh->GetVertexDataPointer(),
            sphereMesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            sphereMesh->GetIndexDataPointer(),
            sphereMesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        auto helmetMesh = load_mesh(".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet.gltf");
        CalculateTangentFrame(helmetMesh);
        auto helmetMeshBuffer = Renderer::CreateMeshBuffer(
            helmetMesh->GetVertexDataPointer(),
            helmetMesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            helmetMesh->GetIndexDataPointer(),
            helmetMesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        auto spitfireMesh = load_mesh(".\\Assets\\Models\\supermarine-spitfire\\spitfire.FBX");
        CalculateTangentFrame(spitfireMesh);
        auto spitfireMeshBuffer = Renderer::CreateMeshBuffer(
            spitfireMesh->GetVertexDataPointer(),
            spitfireMesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            spitfireMesh->GetIndexDataPointer(),
            spitfireMesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        // Add entities to the scene
        { // CAMERA
            FrameBufferParameters fb_params;
            fb_params.width = 1000;
            fb_params.height = 1000;
            fb_params.color_buffer_format = ColorBufferFormat::RGBA8;
            fb_params.depth_buffer_format = DepthBufferFormat::DEPTH24_STENCIL8;
            auto fbuffer = Renderer::CreateFramebuffer(fb_params);

            auto skybox = std::make_shared<Skybox>(main_cubemap, 10.0);

            auto entity = scene.CreateEntity();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto camera_component = entity->AddComponent<CameraComponent>();
            auto camera = std::make_shared<PerspectiveCamera>(glm::pi<float>() / 2.0f, 1.0f, 0.1f, 30.0f);
            transform_comp->transform.Translate(glm::vec3(0.0f, 1.5f, 5.0f));
            camera_component->data.camera = camera;
            camera_component->data.frame_buffer = fbuffer;
            camera_component->data.clear_color = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
            camera_component->data.skybox = skybox;
            camera_transform = transform_comp;

            camera_data = &camera_component->data;
        }
        { // DIRECTIONAL LIGHT
            auto entity = scene.CreateEntity();
            auto light_comp = entity->AddComponent<DirectionalLightComponent>();
            light_comp->data.color = glm::vec3(0.5, 0.5, 0.5);
            light_comp->data.irradiance = 100.0f;
            light_comp->data.direction = glm::normalize(glm::vec3(-0.5, -0.5, 0.0));
        }
        { // HELMET
            auto entity = scene.CreateEntity();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto mesh_comp = entity->AddComponent<MeshRenderComponent>();
            //auto spin_comp = entity->AddComponent<SpinningComponent>();

            transform_comp->transform.Scale(glm::vec3(1.0));
            transform_comp->transform.Translate(glm::vec3(-2.0, 1.5, 0.0));

            mesh_comp->data.mesh_buffer = helmetMeshBuffer;
            mesh_comp->data.material_instance = helmet_material_instance;
        }
        { // SPITFIRE
            auto entity = scene.CreateEntity();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto mesh_comp = entity->AddComponent<MeshRenderComponent>();

            transform_comp->transform.Scale(glm::vec3(0.01));
            transform_comp->transform.Translate(glm::vec3(2.0, 0.5, 0.0));
            transform_comp->transform.Rotate(glm::vec3(1.0, 0.0, 0.0), -glm::pi<float>()*0.5f);

            mesh_comp->data.mesh_buffer = spitfireMeshBuffer;
            mesh_comp->data.material_instance = spitfire_material_instance;
        }
        { // BASE PLATFORM
            auto entity = scene.CreateEntity();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto mesh_comp = entity->AddComponent<MeshRenderComponent>();

            transform_comp->transform.Scale(glm::vec3(10.0f, 0.1f, 10.0));

            mesh_comp->data.mesh_buffer = cube_mbuffer;
            mesh_comp->data.material_instance = metal_plate_material_instance;
        }

        scene.OnAwake();
    }

    virtual void OnAttach()override
    {
    }

    virtual void OnDetatch() override {}

    virtual void OnUpdate(float time_step) override
    {
        scene.OnUpdate(time_step);
        MoveCamera(time_step);

        Renderer::RenderScene(scene.GetRenderData());

        {// Render GUI to main window
            ImGui::Begin("Settings");
            ImGui::SliderFloat("Ambient Light", &m_ambientRadiance, 0.0f, 1.0f);
            scene.SetAmbientLight(m_ambientRadiance);
            ImGui::End();
        }
    }

    virtual void HandleEvent(AppEvent& app_event) override
    {
        app_event.Dispatch<WindowResizedEvent>(
            [this](WindowResizedEvent& e)
            {
                camera_data->camera->SetAspectRatio((float)e.GetWidth() / (float)e.GetHeight());
                return false;
            }
        );

        ME_LOG_INFO(app_event.ToString());
    }

    void MoveCamera(float time_step)
    {
        const auto& up = camera_transform->transform.Up();
        const auto& right = camera_transform->transform.Right();
        const auto& forward = camera_transform->transform.Forward();
        float speed = 5.0e-1f;
        if (Input::GetKeyDown(Key::LEFT_SHIFT))
            speed *= 2.0f;
        if (Input::GetKeyDown(Key::W))
            camera_transform->transform.Translate(+forward * speed * time_step);
        if (Input::GetKeyDown(Key::A))
            camera_transform->transform.Translate(-right * speed * time_step);
        if (Input::GetKeyDown(Key::S))
            camera_transform->transform.Translate(-forward * speed * time_step);
        if (Input::GetKeyDown(Key::D))
            camera_transform->transform.Translate(+right * speed * time_step);
        if (Input::GetKeyDown(Key::Z))
            camera_transform->transform.Translate(+up * speed * time_step);
        if (Input::GetKeyDown(Key::X))
            camera_transform->transform.Translate(-up * speed * time_step);
        if (Input::GetKeyDown(Key::Q))
            camera_transform->transform.Rotate(up, time_step * 1.0);
        if (Input::GetKeyDown(Key::E))
            camera_transform->transform.Rotate(up, -time_step * 1.0);

        camera_transform->transform.Rotate(up, Input::GetMouseScrollDelta().y * time_step * 5.0);
    }
};

class DemoApplication : public Application
{
public:
    DemoApplication(const ApplicationInfo& properties) : Application(properties)
    {
        PushLayerFront(std::make_shared<SceneLayer>());
    }
};


void main()
{
    ApplicationInfo appInfo
    {
        "Demo",
        RenderBackend::OPENGL,
        800,
        800
    };
    DemoApplication app(appInfo);
    app.Run();
}
