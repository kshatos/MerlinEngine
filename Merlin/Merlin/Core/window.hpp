#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <functional>
#include "app_events.hpp"
#include "Merlin/Render/render.hpp"

namespace Merlin
{
    struct WindowProperties
    {
        RenderBackend renderBackend;
        std::string title;
        unsigned int width;
        unsigned int height;

        WindowProperties(
            std::string _title,
            RenderBackend _renderBackend,
            int _width,
            int _height) :
            renderBackend(_renderBackend),
            title(_title),
            width(_width),
            height(_height)
        {
        }
    };

    class Window
    {
    public:
        using EventCallbackFunction = std::function<void(AppEvent&)>;

        static Window* Create(WindowProperties props);

        virtual ~Window() {}
        virtual unsigned int GetWidth() = 0;
        virtual unsigned int GetHeight() = 0;
        virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
        virtual void OnUpdate() = 0;
        virtual void* GetNativePointer() = 0;
    };
}
#endif