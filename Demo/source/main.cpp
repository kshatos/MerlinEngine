#include "Merlin/Core/logger.hpp"
#include "Merlin/Core/window.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/buffer_layout.hpp"
#include "Merlin/Render/buffer_layout.hpp"
#include <sstream>


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
        {ShaderDataType::Float4, "color"}
    };
    float verts[]{ 1.0f, 1.0f, 1.0f };
    auto vbuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(verts, sizeof(verts)));
    vbuffer->SetLayout(layout);

    uint32_t tris[]{ 0, 1, 2 };
    auto ibuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(tris, sizeof(tris) / sizeof(uint32_t)));

    for (const auto& element : layout)
    {
        std::ostringstream oss;
        oss << element.name << ", " << (int)element.type << ", " << element.size << ", " << element.offset << ", " << element.normalized;
        ME_LOG_INFO(oss.str());
    }
    
    while (is_running)
    {
        window->OnUpdate();
    }
}