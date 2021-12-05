#include "Merlin/Core/application.hpp"
#include "Merlin/Core/logger.hpp"
#include "Merlin/Render/renderer.hpp"
#include "imgui.h"
#include <backends/imgui_impl_opengl3.h>


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

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 130");

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

        // IMGUI
        app_event.Dispatch<MouseButtonPressedEvent>(
            [](MouseButtonPressedEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MouseDown[e.GetButton()] = true;
            return false;
        });
        app_event.Dispatch<MouseButtonReleasedEvent>(
            [](MouseButtonReleasedEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MouseDown[e.GetButton()] = false;
            return false;
        });
        app_event.Dispatch<MouseMovedEvent>(
            [](MouseMovedEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MousePos = ImVec2(e.GetX(), e.GetY());
            return false;
        });
        app_event.Dispatch<MouseScrolledEvent>(
            [](MouseScrolledEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MouseWheel += e.GetYScroll();
            io.MouseWheelH += e.GetXScroll();
            return false;
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