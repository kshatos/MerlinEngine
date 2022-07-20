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
        // Initialize render data

        auto texProps = Texture2DProperties(
            TextureWrapMode::Repeat,
            TextureWrapMode::Repeat,
            TextureFilterMode::Linear);
        auto main_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\debug.jpg"), texProps);
        auto pbr_albedo_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Color.jpg"), texProps);
        auto pbr_roughness_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Roughness.jpg"), texProps);
        auto pbr_metalic_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_Metalness.jpg"), texProps);
        auto pbr_normal_texture = Renderer::CreateTexture2D(
            load_texture(".\\Assets\\Textures\\AmbientCG\\MetalPlates007_1K-JPG\\MetalPlates007_1K_NormalGL.jpg"), texProps);

        auto cube_data = std::make_shared<CubemapData>(100, 3);
        for (int face_id = CubeFace::Begin; face_id < CubeFace::End; ++face_id)
        {
            auto face = static_cast<CubeFace>(face_id);
            for (int j = 0; j < 100; ++j)
            {
                for (int i = 0; i < 100; ++i)
                {
                    auto point = CubemapData::CubePoint(cube_data->GetPixelCoordinates(face, i, j));
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
        sphere_mbuffer =  Renderer::CreateMeshBuffer(
            sphereMesh->GetVertexDataPointer(),
            sphereMesh->GetVertexCount() * sizeof(Vertex_XNTBUV),
            sphereMesh->GetIndexDataPointer(),
            sphereMesh->GetTriangleCount() * 3,
            Vertex_XNTBUV::GetLayout());

        // Add entities to the scene
        {
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
        {
            /*
            auto entity = scene.CreateEntity();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto light_comp = entity->AddComponent<SpotLightComponent>();
            light_comp->data.color = glm::vec3(1.0f, 1.0f, 1.0f);
            light_comp->data.cutoffAngle = glm::pi<float>() / 10;
            light_comp->data.falloffRatio = 0.25f;
            light_comp->data.radiantIntensity = 10.0f;
            light_comp->data.range = 50.0f;
            auto follow_cam_comp = entity->AddComponent<FollowCameraComponent>();
            */
        }
        {
            auto entity = scene.CreateEntity();
            auto light_comp = entity->AddComponent<DirectionalLightComponent>();
            light_comp->data.color = glm::vec3(0.2, 0.2, 1.0);
            light_comp->data.irradiance = 100.0f;
            light_comp->data.direction = glm::normalize(glm::vec3(-0.5, -0.5, 0.0));
        }
        /*
        for (int i = 0; i < 4; ++i)
        {
            auto entity = scene.CreateEntity();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            transform_comp->transform.Translate(glm::vec3(
                glm::linearRand(-2.0f, 2.0f),
                glm::linearRand(-2.0f, 2.0f),
                5.0f));
            auto light_comp = entity->AddComponent<PointLightComponent>();
            light_comp->data.color = glm::vec3(1.0f, 1.0f, 1.0f);
            light_comp->data.radiantFlux = 50.0f;
            light_comp->data.range = 50.0f;
        }
        */
        {
            auto entity = scene.CreateEntity();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto mesh_comp = entity->AddComponent<MeshRenderComponent>();
            auto spin_comp = entity->AddComponent<SpinningComponent>();

            transform_comp->transform.Translate(glm::vec3(0.0, 1.5, 0.0));

            mesh_comp->data.mesh_buffer = cube_mbuffer;
            mesh_comp->data.material = pbr_texture_material;
        }
        {
            auto entity = scene.CreateEntity();
            auto transform_comp = entity->AddComponent<TransformComponent>();
            auto mesh_comp = entity->AddComponent<MeshRenderComponent>();

            transform_comp->transform.Scale(glm::vec3(5.0f, 0.1f, 5.0));

            mesh_comp->data.mesh_buffer = cube_mbuffer;
            mesh_comp->data.material = pbr_texture_material;
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
