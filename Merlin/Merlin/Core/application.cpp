#include "Merlin/Core/application.hpp"
#include "Merlin/Core/logger.hpp"
#include "Merlin/Render/renderer.hpp"
#include "Merlin/Core/imgui_layer.hpp"


namespace Merlin
{
    Application* Application::app_instance = nullptr;

    Application::Application()
    {
        Logger::Init();

        main_window = std::unique_ptr<Window>(Window::Create(WindowProperties("asdf", 800, 800)));
        main_window->SetEventCallback(
            [this](AppEvent& e) { return this->HandleEvent(e); });

        Renderer::Init();

        PushLayerFront(std::make_shared<IMGUILayer>());

        is_running = true;
        app_instance = this;
    }

    void Application::PushLayerFront(std::shared_ptr<Layer> layer)
    {
        layer_stack.PushFront(layer);
    }

    void Application::PushLayerBack(std::shared_ptr<Layer> layer)
    {
        layer_stack.PushFront(layer);
    }

    void Application::HandleEvent(AppEvent& app_event)
    {
        app_event.Dispatch<WindowClosedEvent>(
            [this](WindowClosedEvent& e)
        {
            is_running = false;
            return true;
        });

        // Dispatch events to layers allowing them to block
        for (auto& layer : layer_stack)
        {
            if (app_event.was_handled)
                break;
            layer->HandleEvent(app_event);
        }
    }

    void Application::OnUpdate()
    {
        for(auto & layer : layer_stack)
        {
            layer->OnUpdate();
        }
    }

    void Application::Run()
    {
        while (is_running)
        {
            OnUpdate();
            main_window->OnUpdate();
        }
    }

}