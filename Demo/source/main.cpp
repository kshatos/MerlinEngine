#include <sstream>
#include "Merlin/Core/logger.hpp"
#include "Merlin/Core/window.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/buffer_layout.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include <glad/glad.h>


using namespace Merlin;

bool is_running = true;

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
    Logger::Init();

    auto window = std::unique_ptr<Window>(Window::Create(WindowProperties("asdf", 600, 800)));
    window->SetEventCallback(EventCallback);

    BufferLayout layout{
        {ShaderDataType::Float3, "pos"},
    };
    float verts[]
    {
        -0.5f, -0.5f, +0.0f,
        +0.5f, -0.5f, +0.0f,
        +0.5f, +0.5f, +0.0f,
        -0.5f, +0.5f, +0.0f
    };
    auto vbuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(verts, sizeof(verts)));
    vbuffer->SetLayout(layout);

    uint32_t tris[]
    {
        0, 1, 2,
        0, 2, 3
    };
    auto ibuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(tris, sizeof(tris) / sizeof(uint32_t)));

    auto varray = std::unique_ptr<VertexArray>(VertexArray::Create());
    varray->AddVertexBuffer(vbuffer);
    varray->SetIndexBuffer(ibuffer);
    
    while (is_running)
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        varray->Bind();
        glDrawElements(GL_TRIANGLES, varray->GetIndexBuffer()->GetCount() , GL_UNSIGNED_INT, nullptr);
        window->OnUpdate();
    }
}