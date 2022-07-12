#include <glad/glad.h>
#include "Merlin/Platform/OpenGL/opengl_render_api.hpp"
#include "Merlin/Core/logger.hpp"
#include <backends/imgui_impl_opengl3.h>
#include "backends/imgui_impl_glfw.h"
#include "Merlin/Platform/OpenGL/opengl_vertex_buffer.hpp"
#include "Merlin/Platform/OpenGL/opengl_index_buffer.hpp"
#include "Merlin/Platform/OpenGL/opengl_vertex_array.hpp"
#include "Merlin/Platform/OpenGL/opengl_shader.hpp"
#include "Merlin/Platform/OpenGL/opengl_texture2d.hpp"
#include "Merlin/Platform/OpenGL/opengl_cubemap.hpp"
#include "Merlin/Platform/OpenGL/opengl_frame_buffer.hpp"



namespace Merlin
{
    void GLAPIENTRY
        LogOpenGLErrors(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
    {
        ME_LOG_INFO(std::string(message));
    }

    void OpenGLRenderAPI::Init(void* windowPointer)
    {
        m_window = (GLFWwindow*)windowPointer;

        glfwMakeContextCurrent(m_window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        IMGUI_CHECKVERSION();
        context = ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)windowPointer, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(LogOpenGLErrors, 0);

        m_shadow_shader = CreateShader(
            ".\\Assets\\Shaders\\shadow.vert",
            ".\\Assets\\Shaders\\shadow.frag");
        m_skybox_shader = CreateShader(
            ".\\Assets\\Shaders\\skybox.vert",
            ".\\Assets\\Shaders\\skybox.frag");
        m_shadow_buffer = CreateFramebuffer(
            FrameBufferParameters
            {
                2048, 2048,
                ColorBufferFormat::NONE,
                DepthBufferFormat::DEPTH32
            });
    }

    void OpenGLRenderAPI::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(context);
    }

    void OpenGLRenderAPI::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void OpenGLRenderAPI::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void OpenGLRenderAPI::PresentFrame()
    {
        glfwSwapBuffers(m_window);
    }

    void OpenGLRenderAPI::RenderScene(const SceneRenderData& scene)
    {
        if (scene.camera == nullptr)
            return;

        // Shadow map depth pass
        {
            auto shadow_buffer = std::dynamic_pointer_cast<OpenGLFrameBuffer>(m_shadow_buffer);
            auto buffer_params = shadow_buffer->GetParameters();
            shadow_buffer->Bind();
            glViewport(0, 0, buffer_params.width, buffer_params.height);
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            DrawMeshShadows(scene);
            shadow_buffer->UnBind();
        }

        // Final lighting pass
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(
                scene.camera->clear_color.r,
                scene.camera->clear_color.g,
                scene.camera->clear_color.b,
                scene.camera->clear_color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            DrawMeshes(scene);
            DrawSkybox(scene);
        }
    }

    void OpenGLRenderAPI::BindMaterial(
        const std::shared_ptr<Material>& material)
    {
        auto shader = std::dynamic_pointer_cast<OpenGLShader>(material->m_shader);
        shader->Bind();
        for (int i = 0; i < material->m_textureData.size(); ++i)
        {
            if (material->m_textureData[i])
            {
                std::dynamic_pointer_cast<OpenGLTexture2D>(material->m_textureData[i])->Bind(i);
                material->m_shader->SetUniformInt(material->m_textureNames[i], i);
            }
        }
        for (const auto& item : material->m_uniformLayout)
        {
            shader->SetUniform(item.name, item.type, material->m_uniformData + item.offset);
        }
    }

    void OpenGLRenderAPI::DrawMeshes(const SceneRenderData& scene)
    {
        auto& camera_data = *scene.camera;
        auto shadow_buffer = std::dynamic_pointer_cast<OpenGLFrameBuffer>(m_shadow_buffer);
        for (const auto& mesh_pointer : scene.meshes)
        {
            MeshRenderData& data = *mesh_pointer;

            const auto& material = data.material;
            const auto& vertex_array = std::dynamic_pointer_cast<OpenGLVertexArray>(data.vertex_array);
            const auto& model_matrix = data.model_matrix;

            BindMaterial(material);
            material->SetUniformFloat3("u_viewPos", camera_data.view_pos);
            material->SetUniformMat3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model_matrix))));
            material->SetUniformMat4("u_ModelMatrix", model_matrix);
            material->SetUniformMat4("u_ViewMatrix", camera_data.view_matrix);
            material->SetUniformMat4("u_ProjectionMatrix", camera_data.projection_matrix);

            if (scene.directional_lights.size() > 0)
            {
                auto light_matrix = GetLightMatrix(camera_data, *scene.directional_lights[0]);
                material->SetUniformMat4("u_lightTransform", light_matrix);
            }
            uint32_t shadow_slot = material->TextureCount() + 1;
            shadow_buffer->BindDepthTexture(shadow_slot);
            material->SetUniformInt("u_shadowBufferTexture", shadow_slot);

            material->SetUniformFloat("u_ambientRadiance", scene.ambient_light_radiance);

            material->SetUniformInt("u_nPointLights", scene.point_lights.size());
            for (int i = 0; i < scene.point_lights.size(); ++i)
            {
                const auto& light = *scene.point_lights[i];
                material->SetUniformFloat3("u_pointLights[" + std::to_string(i) + "].position", light.position);
                material->SetUniformFloat("u_pointLights[" + std::to_string(i) + "].radiantFlux", light.radiantFlux);
                material->SetUniformFloat("u_pointLights[" + std::to_string(i) + "].range", light.range);
                material->SetUniformFloat3("u_pointLights[" + std::to_string(i) + "].color", light.color);
            }

            material->SetUniformInt("u_nDirectionalLights", scene.directional_lights.size());
            for (int i = 0; i < scene.directional_lights.size(); ++i)
            {
                const auto& light = *scene.directional_lights[i];
                material->SetUniformFloat3("u_directionalLights[" + std::to_string(i) + "].direction", light.direction);
                material->SetUniformFloat("u_directionalLights[" + std::to_string(i) + "].irradiance", light.irradiance);
                material->SetUniformFloat3("u_directionalLights[" + std::to_string(i) + "].color", light.color);
            }

            material->SetUniformInt("u_nSpotLights", scene.spot_lights.size());
            for (int i = 0; i < scene.spot_lights.size(); ++i)
            {
                const auto& light = *scene.spot_lights[i];
                material->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].position", light.position);
                material->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].direction", light.direction);
                material->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].cutoffAngle", light.cutoffAngle);
                material->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].falloffRatio", light.falloffRatio);
                material->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].radiantIntensity", light.radiantIntensity);
                material->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].range", light.range);
                material->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].color", light.color);
            }

            vertex_array->Bind();
            glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            vertex_array->UnBind();
        }
    }

    void OpenGLRenderAPI::DrawSkybox(const SceneRenderData& scene)
    {
        auto& camera = *scene.camera;
        auto skybox = camera.skybox;
        auto skybox_shader = std::dynamic_pointer_cast<OpenGLShader>(m_skybox_shader);
        skybox_shader->Bind();
        if (skybox)
        {
            auto& cubemap = std::dynamic_pointer_cast<OpenGLCubemap>(skybox->GetCubemap());
            auto& varray = std::dynamic_pointer_cast<OpenGLVertexArray>(skybox->GetVertexArray());

            cubemap->Bind(0);

            skybox_shader->SetUniformFloat3("u_viewPos", camera.view_pos);
            skybox_shader->SetUniformMat4("u_ViewMatrix", glm::mat4(glm::mat3(camera.view_matrix)));
            skybox_shader->SetUniformMat4("u_ProjectionMatrix", camera.projection_matrix);

            varray->Bind();

            glDrawElements(GL_TRIANGLES, varray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            varray->UnBind();
            cubemap->UnBind(0);
        }
        skybox_shader->UnBind();
    }

    void OpenGLRenderAPI::DrawMeshShadows(const SceneRenderData& scene)
    {
        if (scene.directional_lights.size() < 1)
            return;

        auto& light_data = *scene.directional_lights[0];
        auto& camera_data = *scene.camera;

        auto light_matrix = GetLightMatrix(camera_data, light_data);


        auto shadow_shader = std::dynamic_pointer_cast<OpenGLShader>(m_shadow_shader);
        shadow_shader->Bind();
        for (const auto& mesh_pointer : scene.meshes)
        {
            MeshRenderData& data = *mesh_pointer;

            const auto& vertex_array = std::dynamic_pointer_cast<OpenGLVertexArray>(data.vertex_array);
            const auto& model_matrix = data.model_matrix;

            shadow_shader->SetUniformMat4("u_ModelMatrix", model_matrix);
            shadow_shader->SetUniformMat4("u_LightSpaceMatrix", light_matrix);

            vertex_array->Bind();
            glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            vertex_array->UnBind();
        }
        shadow_shader->UnBind();
    }

    RenderBackend OpenGLRenderAPI::Backend()
    {
        return RenderBackend::OPENGL;
    }


    std::shared_ptr<VertexBuffer> OpenGLRenderAPI::CreateVertexBuffer(
        float* vertices, size_t size)
    {
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    std::shared_ptr<IndexBuffer> OpenGLRenderAPI::CreateIndexBuffer(
        uint32_t* indices, uint32_t index_count)
    {
        return std::make_shared<OpenGLIndexBuffer>(indices, index_count);
    }

    std::shared_ptr<VertexArray> OpenGLRenderAPI::CreateVertexArray()
    {
        return std::make_shared<OpenGLVertexArray>();
    }

    std::shared_ptr<Shader> OpenGLRenderAPI::CreateShader(
        const std::string& vertex_source,
        const std::string& fragment_source)
    {
        return std::shared_ptr<OpenGLShader>(
            OpenGLShader::CreateFromFiles(
                vertex_source, fragment_source));
    }

    std::shared_ptr<Texture2D> OpenGLRenderAPI::CreateTexture2D(
        const std::string& filepath,
        Texture2DProperties props)
    {
        return std::make_shared<OpenGLTexture2D>(
            filepath, props);
    }

    std::shared_ptr<Texture2D> OpenGLRenderAPI::CreateTexture2D(
        void* data,
        uint32_t width,
        uint32_t height,
        uint32_t channel_count,
        Texture2DProperties props)
    {
        return std::make_shared<OpenGLTexture2D>(
            data, width, height, channel_count, props);
    }

    std::shared_ptr<Cubemap> OpenGLRenderAPI::CreateCubemap(
        const std::vector<std::string>& face_paths)
    {
        return std::make_shared<OpenGLCubemap>(face_paths);
    }

    std::shared_ptr<Cubemap> OpenGLRenderAPI::CreateCubemap(
        uint32_t resolution, uint32_t channel_count)
    {
        return std::make_shared<OpenGLCubemap>(
            resolution, channel_count);
    }

    std::shared_ptr<FrameBuffer> OpenGLRenderAPI::CreateFramebuffer(
        const FrameBufferParameters& state)
    {
        return std::make_shared<OpenGLFrameBuffer>(state);
    }

}
