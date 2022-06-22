#include "Merlin/Core/imgui_layer.hpp"
#include "imgui.h"
#ifdef MERLIN_USE_OPENGL
#include <backends/imgui_impl_opengl3.h>
#endif
#ifdef MERLIN_USE_VULKAN
#include <backends/imgui_impl_vulkan.h>
#endif

namespace Merlin
{
    void IMGUILayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void IMGUILayer::OnDetatch()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    void IMGUILayer::OnUpdate(float time_step)
    {
    }

    void IMGUILayer::HandleEvent(AppEvent& app_event)
    {
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
            io.MousePos = ImVec2(e.GetXPosition(), e.GetYPosition());
            return false;
        });
        app_event.Dispatch<MouseScrolledEvent>(
            [](MouseScrolledEvent& e)
        {
            auto& io = ImGui::GetIO();
            io.MouseWheel += e.GetYScrollDelta();
            io.MouseWheelH += e.GetXScrollDelta();
            return false;
        });
    }

}