#include <sstream>
#include "Merlin/Core/logger.hpp"
#include "Merlin/Core/window.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/buffer_layout.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/shader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace Merlin;

bool is_running = true;

float verts[]
{
    -0.5f, -0.5f, +0.0f, 1.0f, 0.0f , 0.0f, 1.0f,
    +0.5f, -0.5f, +0.0f, 0.0f, 1.0f , 0.0f, 1.0f,
    +0.5f, +0.5f, +0.0f, 0.0f, 0.0f , 1.0f, 1.0f,
    -0.5f, +0.5f, +0.0f, 0.0f, 0.0f , 0.0f, 1.0f
};

uint32_t tris[]
{
    0, 1, 2,
    0, 2, 3
};

auto vertex_source =
R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 color;


void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   color = aCol;
}
)";

auto fragment_source =
R"(
#version 330 core
out vec4 FragColor;
in vec3 color;


void main()
{
    FragColor = vec4(color, 1.0f);
}
)";


void EventCallback(AppEvent& app_event)
{
    ME_LOG_INFO(app_event.ToString());

    AppEvent::Dispatch<WindowClosedEvent>(app_event,
        [](WindowClosedEvent& e)
    {
        is_running = false;
        return true;
    });
}

void main()
{
    glm::vec2 x(0.0f, 1.0f);

    Logger::Init();

    auto window = std::unique_ptr<Window>(Window::Create(WindowProperties("asdf", 600, 800)));
    window->SetEventCallback(EventCallback);

    auto shader = std::shared_ptr<Shader>(Shader::Create(vertex_source, fragment_source));

    BufferLayout layout{
        {ShaderDataType::Float3, "pos"},
        {ShaderDataType::Float4, "color"},
    };

    auto vbuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(verts, sizeof(verts)));
    vbuffer->SetLayout(layout);

    auto ibuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(tris, sizeof(tris) / sizeof(uint32_t)));

    auto varray = std::unique_ptr<VertexArray>(VertexArray::Create());
    varray->AddVertexBuffer(vbuffer);
    varray->SetIndexBuffer(ibuffer);

    while (is_running)
    {
        glViewport(0, 0, window->GetWidth(), window->GetHeight());
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader->Bind();
        varray->Bind();
        glDrawElements(GL_TRIANGLES, varray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        window->OnUpdate();
    }
}