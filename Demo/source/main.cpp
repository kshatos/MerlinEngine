#include "Merlin/Core/logger.hpp"
#include "Merlin/Core/window.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/buffer_layout.hpp"
#include "Merlin/Render/buffer_layout.hpp"

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

    float verts[]{ 1.0f, 1.0f, 1.0f };
    auto vbuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(verts, sizeof(verts)));

    uint32_t tris[]{ 0, 1, 2 };
    auto ibuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(tris, sizeof(tris) / sizeof(uint32_t)));

    BufferLayout layout{
        {ShaderDataType::Float3, "pos"},
        {ShaderDataType::Float4, "color"}
    };

    while (is_running)
    {
        window->OnUpdate();
    }
}