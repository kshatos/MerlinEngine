#include "Merlin/Core/application.hpp"
#include "Merlin/Core/logger.hpp"
#include "Merlin/Render/renderer.hpp"
#include "Merlin/Core/input.hpp"


namespace Merlin
{
    Application* Application::app_instance = nullptr;

    Application::Application(const ApplicationInfo& properties)
    {
        Logger::Init();

        WindowProperties windowProps
        {
            properties.appName,
            properties.renderBackend,
            properties.windowWidth,
            properties.windowHeight
        };
        m_main_window = std::unique_ptr<Window>(Window::Create(windowProps));
        m_main_window->SetEventCallback(
            [this](AppEvent& e) { return this->HandleEvent(e); });

        Renderer::Init(m_main_window->m_renderApi);

        m_is_running = true;
        app_instance = this;
    }

    void Application::PushLayerFront(std::shared_ptr<Layer> layer)
    {
        m_layer_stack.PushFront(layer);
    }

    void Application::PushLayerBack(std::shared_ptr<Layer> layer)
    {
        m_layer_stack.PushFront(layer);
    }

    void Application::HandleEvent(AppEvent& app_event)
    {
        app_event.Dispatch<WindowClosedEvent>(
            [this](WindowClosedEvent& e)
        {
            m_is_running = false;
            return true;
        });

        Input::HandleEvent(app_event);

        m_main_window->HandleEvent(app_event);

        // Dispatch events to layers allowing them to block
        for (auto& layer : m_layer_stack)
        {
            if (app_event.m_was_handled)
                break;
            layer->HandleEvent(app_event);
        }
    }

    void Application::OnUpdate(float time_step)
    {
        for(auto & layer : m_layer_stack)
        {
            layer->OnUpdate(time_step);
        }
        Input::EndFrame();
    }

    void Application::Run()
    {
        while (m_is_running)
        {
            auto current_frame_time = m_main_window->CurrentTime();
            auto time_step = current_frame_time - m_last_frame_time;
            m_last_frame_time = current_frame_time;

            OnUpdate(time_step);
            m_main_window->OnUpdate();
        }
    }

}