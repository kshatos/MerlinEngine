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

        main_window= std::unique_ptr<Window>(Window::Create(WindowProperties("asdf", 800, 800)));
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

    void Application::HandleEvent(AppEvent& app_event)
    {
        AppEvent::Dispatch<WindowClosedEvent>(app_event,
            [this](WindowClosedEvent& e)
        {
            is_running = false;
            return true;
        });

        // IMGUI
        AppEvent::Dispatch<MouseButtonPressedEvent>(app_event,
            [](MouseButtonPressedEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MouseDown[e.GetButton()] = true;
            return false;
        });
        AppEvent::Dispatch<MouseButtonReleasedEvent>(app_event,
            [](MouseButtonReleasedEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MouseDown[e.GetButton()] = false;
            return false;
        });
        AppEvent::Dispatch<MouseMovedEvent>(app_event,
            [](MouseMovedEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MousePos = ImVec2(e.GetX(), e.GetY());
            return false;
        });
        AppEvent::Dispatch<MouseScrolledEvent>(app_event,
            [](MouseScrolledEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MouseWheel += e.GetYScroll();
            io.MouseWheelH += e.GetXScroll();
            return false;
        });
    }

    void Application::Run()
    {
        while (is_running)
        {
            this->OnUpdate();
            main_window->OnUpdate();
        }
    }

}