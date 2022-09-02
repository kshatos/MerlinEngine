#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <sstream>

#include "Merlin/Core/core.hpp"
#include "Merlin/Core/file_util.hpp"
#include "Merlin/Render/render.hpp"
#include "Merlin/Scene/scene.hpp"
#include "uv_sphere.hpp"

using namespace Merlin;

Vertex_XNTBUV cube_verts[]{
    // positions             // normal              // texture coords
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},

    {0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},

    {0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},

    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},

    {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},

    {0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f},
};

uint32_t cube_indices[]{0,  1,  2,  0,  2,  3,

                        4,  5,  6,  4,  6,  7,

                        8,  9,  10, 8,  10, 11,

                        12, 13, 14, 12, 14, 15,

                        16, 17, 18, 16, 18, 19,

                        20, 21, 22, 20, 22, 23};

std::shared_ptr<MeshBuffer> cube_mbuffer;
std::shared_ptr<MeshBuffer> sphere_mbuffer;
std::shared_ptr<Cubemap> main_cubemap;
std::shared_ptr<Material> main_material;
std::shared_ptr<Material> pbr_texture_material;


class SceneLayer : public Layer
{
    float m_ambient_radiance = 0.0f;
    GameScene scene;
    Transform* camera_transform;
    ImVec2 viewport_size{0, 0};

public:
    SceneLayer()
    {
        auto tex_props = Texture2DProperties(TextureWrapMode::Repeat,
                                             TextureWrapMode::Repeat,
                                             TextureFilterMode::Linear);

        auto main_texture = Renderer::CreateTexture2D(
            LoadTexture(".\\Assets\\Textures\\debug.jpg"), tex_props);

        auto metal_plate_albedo_texture = Renderer::CreateTexture2D(
            LoadTexture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-"
                        "JPG\\MetalPlates007_1K_Color.jpg"),
            tex_props);
        auto metal_plate_roughness_texture = Renderer::CreateTexture2D(
            LoadTexture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-"
                        "JPG\\MetalPlates007_1K_Roughness.jpg"),
            tex_props);
        auto metal_plate_metalic_texture = Renderer::CreateTexture2D(
            LoadTexture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-"
                        "JPG\\MetalPlates007_1K_Metalness.jpg"),
            tex_props);
        auto metal_plate_normal_texture = Renderer::CreateTexture2D(
            LoadTexture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-"
                        "JPG\\MetalPlates007_1K_NormalGL.jpg"),
            tex_props);

        auto helmet_albedo_texture = Renderer::CreateTexture2D(
            LoadTexture(
                ".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet_BaseColor.png"),
            tex_props);
        auto helmet_roughness_texture = Renderer::CreateTexture2D(
            LoadTexture(".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet_"
                        "MetallicRoughness.png"),
            tex_props);
        auto helmet_metalic_texture = Renderer::CreateTexture2D(
            LoadTexture(".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet_"
                        "MetallicRoughness.png"),
            tex_props);
        auto helmet_normal_texture = Renderer::CreateTexture2D(
            LoadTexture(
                ".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet_Normal.png"),
            tex_props);

        auto spitfire_albedo_texture = Renderer::CreateTexture2D(
            LoadTexture(
                ".\\Assets\\Models\\supermarine-spitfire\\spitfire_d.png"),
            tex_props);
        auto spitfire_roughness_texture = Renderer::CreateTexture2D(
            LoadTexture(
                ".\\Assets\\Models\\supermarine-spitfire\\spitfire_r.png"),
            tex_props);
        auto spitfire_metalic_texture = Renderer::CreateTexture2D(
            LoadTexture(
                ".\\Assets\\Models\\supermarine-spitfire\\spitfire_m.png"),
            tex_props);
        auto spitfire_normal_texture = Renderer::CreateTexture2D(
            LoadTexture(
                ".\\Assets\\Models\\supermarine-spitfire\\spitfire_n.png"),
            tex_props);

        main_cubemap = Renderer::CreateCubemap(std::vector<std::string>{
            ".\\Assets\\Textures\\skybox_hilly_lake\\right.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\left.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\top.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\bottom.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\front.jpg",
            ".\\Assets\\Textures\\skybox_hilly_lake\\back.jpg"});

        auto pbr_shader =
            Renderer::CreateShader(".\\Assets\\Shaders\\pbr_lit_basic.vert",
                                   ".\\Assets\\Shaders\\pbr_lit_basic.frag");

        auto pbr_texture_shader =
            Renderer::CreateShader(".\\Assets\\Shaders\\pbr_lit_texture.vert",
                                   ".\\Assets\\Shaders\\pbr_lit_texture.frag");

        auto main_shader =
            Renderer::CreateShader(".\\Assets\\Shaders\\basic_lit.vert",
                                   ".\\Assets\\Shaders\\basic_lit.frag");

        pbr_texture_material = Renderer::CreateMaterial(
            MaterialInfo{pbr_texture_shader,
                         BufferLayout{},
                         std::vector<std::string>{"u_albedoTexture",
                                                  "u_roughnessTexture",
                                                  "u_metalicTexture",
                                                  "u_normalTexture"}});
        auto metal_plate_material_instance =
            Renderer::CreateMaterialInstance(pbr_texture_material);
        metal_plate_material_instance->SetTexture("u_albedoTexture",
                                                  metal_plate_albedo_texture);
        metal_plate_material_instance->SetTexture(
            "u_roughnessTexture", metal_plate_roughness_texture);
        metal_plate_material_instance->SetTexture("u_metalicTexture",
                                                  metal_plate_metalic_texture);
        metal_plate_material_instance->SetTexture("u_normalTexture",
                                                  metal_plate_normal_texture);

        auto helmet_material_instance =
            Renderer::CreateMaterialInstance(pbr_texture_material);
        helmet_material_instance->SetTexture("u_albedoTexture",
                                             helmet_albedo_texture);
        helmet_material_instance->SetTexture("u_roughnessTexture",
                                             helmet_roughness_texture);
        helmet_material_instance->SetTexture("u_metalicTexture",
                                             helmet_metalic_texture);
        helmet_material_instance->SetTexture("u_normalTexture",
                                             helmet_normal_texture);

        auto spitfire_material_instance =
            Renderer::CreateMaterialInstance(pbr_texture_material);
        spitfire_material_instance->SetTexture("u_albedoTexture",
                                               spitfire_albedo_texture);
        spitfire_material_instance->SetTexture("u_roughnessTexture",
                                               spitfire_roughness_texture);
        spitfire_material_instance->SetTexture("u_metalicTexture",
                                               spitfire_metalic_texture);
        spitfire_material_instance->SetTexture("u_normalTexture",
                                               spitfire_normal_texture);

        auto cube_mesh = std::make_shared<Mesh<Vertex_XNTBUV>>();
        cube_mesh->SetVertexData(cube_verts,
                                 sizeof(cube_verts) / sizeof(Vertex_XNUV));
        cube_mesh->SetIndexData(cube_indices,
                                sizeof(cube_indices) / sizeof(uint32_t));
        CalculateTangentFrame(cube_mesh);
        cube_mbuffer = Renderer::CreateMeshBuffer(
            cube_mesh->GetVertexDataPointer(),
            cube_mesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            cube_mesh->GetIndexDataPointer(),
            cube_mesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        auto sphere_mesh = std::make_shared<Mesh<Vertex_XNTBUV>>();
        sphere_mesh->SetVertexData(
            UVSphereVerts, sizeof(UVSphereVerts) / sizeof(Vertex_XNTBUV));
        sphere_mesh->SetIndexData(UVSphereIndices,
                                  sizeof(UVSphereIndices) / sizeof(uint32_t));
        CalculateTangentFrame(sphere_mesh);
        sphere_mbuffer = Renderer::CreateMeshBuffer(
            sphere_mesh->GetVertexDataPointer(),
            sphere_mesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            sphere_mesh->GetIndexDataPointer(),
            sphere_mesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        auto helmet_mesh =
            LoadMesh(".\\Assets\\Models\\SciFiHelmet\\SciFiHelmet.gltf");
        CalculateTangentFrame(helmet_mesh);
        auto helmet_mesh_buffer = Renderer::CreateMeshBuffer(
            helmet_mesh->GetVertexDataPointer(),
            helmet_mesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            helmet_mesh->GetIndexDataPointer(),
            helmet_mesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        auto spitfire_mesh =
            LoadMesh(".\\Assets\\Models\\supermarine-spitfire\\spitfire.FBX");
        CalculateTangentFrame(spitfire_mesh);
        auto spitfire_mesh_buffer = Renderer::CreateMeshBuffer(
            spitfire_mesh->GetVertexDataPointer(),
            spitfire_mesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            spitfire_mesh->GetIndexDataPointer(),
            spitfire_mesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        // Add entities to the scene
        {  // CAMERA
            FrameBufferParameters fb_params;
            fb_params.width = 1000;
            fb_params.height = 1000;
            fb_params.color_buffer_format = ColorBufferFormat::RGBA8;
            fb_params.depth_buffer_format = DepthBufferFormat::Depth24Stencil8;
            auto fbuffer = Renderer::CreateFramebuffer(fb_params);

            auto skybox = std::make_shared<Skybox>(main_cubemap, 10.0);

            auto& entity = scene.CreateEntity();
            auto& transform_comp = entity.GetComponent<TransformComponent>();
            auto& camera_component = entity.AddComponent<CameraComponent>();
            auto camera = std::make_shared<PerspectiveCamera>(
                glm::pi<float>() / 2.0f, 1.0f, 0.1f, 30.0f);
            transform_comp.transform.Translate(glm::vec3(0.0f, 1.5f, 5.0f));
            camera_component.camera_data.camera = camera;
            camera_component.camera_data.frame_buffer = fbuffer;
            camera_component.camera_data.clear_color =
                glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
            camera_component.camera_data.skybox = skybox;

            camera_transform = &transform_comp.transform;
        }
        {  // DIRECTIONAL LIGHT
            auto& entity = scene.CreateEntity();
            auto& light_comp = entity.AddComponent<DirectionalLightComponent>();
            light_comp.m_data.color = glm::vec3(0.5, 0.5, 0.5);
            light_comp.m_data.irradiance = 100.0f;
            light_comp.m_data.direction =
                glm::normalize(glm::vec3(-0.5, -0.5, 0.0));
        }
        {  // HELMET
            auto& entity = scene.CreateEntity();
            auto& transform_comp = entity.GetComponent<TransformComponent>();
            auto& mesh_comp = entity.AddComponent<MeshRenderComponent>();

            transform_comp.transform.Scale(glm::vec3(1.0));
            transform_comp.transform.Translate(glm::vec3(-2.0, 1.5, 0.0));

            mesh_comp.m_data.mesh_buffer = helmet_mesh_buffer;
            mesh_comp.m_data.material_instance = helmet_material_instance;
        }
        {  // SPITFIRE
            auto& entity = scene.CreateEntity();
            auto& transform_comp = entity.GetComponent<TransformComponent>();
            auto& mesh_comp = entity.AddComponent<MeshRenderComponent>();

            transform_comp.transform.Scale(glm::vec3(0.01));
            transform_comp.transform.Translate(glm::vec3(2.0, 0.5, 0.0));
            transform_comp.transform.Rotate(glm::vec3(1.0, 0.0, 0.0),
                                               -glm::pi<float>() * 0.5f);

            mesh_comp.m_data.mesh_buffer = spitfire_mesh_buffer;
            mesh_comp.m_data.material_instance = spitfire_material_instance;
        }
        {  // BASE PLATFORM
            auto& entity = scene.CreateEntity();
            auto& transform_comp = entity.GetComponent<TransformComponent>();
            auto& mesh_comp = entity.AddComponent<MeshRenderComponent>();

            transform_comp.transform.Scale(glm::vec3(10.0f, 0.1f, 10.0));

            mesh_comp.m_data.mesh_buffer = cube_mbuffer;
            mesh_comp.m_data.material_instance = metal_plate_material_instance;
        }

        scene.OnAwake();
    }

    virtual void OnAttach() override {}

    virtual void OnDetatch() override {}

    virtual void OnUpdate(float time_step) override
    {
        scene.OnUpdate(time_step);
        MoveCamera(time_step);

        Renderer::RenderScene(scene.GetRenderData());

        {  // Render GUI to main window
            ImGui::Begin("Settings");
            ImGui::SliderFloat(
                "Ambient Light", &m_ambient_radiance, 0.0f, 1.0f);
            scene.SetAmbientLight(m_ambient_radiance);
            ImGui::End();

            ImGui::Begin("Dockable");
            ImGui::End();
        }
    }

    virtual void HandleEvent(AppEvent& app_event) override
    {
        app_event.Dispatch<WindowResizedEvent>(
            [this](WindowResizedEvent& e)
            {
                auto& render_data = scene.GetRenderData();
                
                render_data.camera->camera->SetAspectRatio((float)e.GetWidth() /
                                                    (float)e.GetHeight());
                return false;
            });

        ME_LOG_INFO(app_event.ToString());
    }

    void MoveCamera(float time_step)
    {
        const auto& up = camera_transform->Up();
        const auto& right = camera_transform->Right();
        const auto& forward = camera_transform->Forward();
        float speed = 5.0e-1f;
        if (Input::GetKeyDown(Key::LEFT_SHIFT)) speed *= 2.0f;
        if (Input::GetKeyDown(Key::W))
            camera_transform->Translate(+forward * speed *
                                                    time_step);
        if (Input::GetKeyDown(Key::A))
            camera_transform->Translate(-right * speed * time_step);
        if (Input::GetKeyDown(Key::S))
            camera_transform->Translate(-forward * speed *
                                                    time_step);
        if (Input::GetKeyDown(Key::D))
            camera_transform->Translate(+right * speed * time_step);
        if (Input::GetKeyDown(Key::Z))
            camera_transform->Translate(+up * speed * time_step);
        if (Input::GetKeyDown(Key::X))
            camera_transform->Translate(-up * speed * time_step);
        if (Input::GetKeyDown(Key::Q))
            camera_transform->Rotate(up, time_step * 1.0);
        if (Input::GetKeyDown(Key::E))
            camera_transform->Rotate(up, -time_step * 1.0);

        camera_transform->Rotate(
            up, Input::GetMouseScrollDelta().y * time_step * 5.0);
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
    ApplicationInfo app_info{
        "Demo",
        RenderBackend::OpenGL,
        800,
        800,
    };
    DemoApplication app(app_info);
    app.Run();
}
