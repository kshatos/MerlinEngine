#ifndef APP_EVENTS_HPP
#define APP_EVENTS_HPP
#include <string>
#include <sstream>


namespace Merlin
{
    enum class AppEventType
    {
        None = 0,
        WindowClosed,
        WindowResized
    };

    class AppEvent
    {
    public:
        virtual ~AppEvent() {}
        virtual AppEventType GetType() = 0;
        virtual std::string ToString() = 0;
    protected:
        bool was_handled = false;
    };

    // WINDOW EVENTS
    class WindowClosedEvent : public AppEvent
    {
    public:
        static AppEventType GetStaticType() { return AppEventType::WindowClosed; }
        inline AppEventType GetType() override { return AppEventType::WindowClosed; };
        std::string ToString() override { return "WindowClosedEvent"; }
    };

    class WindowResizedEvent : public AppEvent
    {
        unsigned int new_width;
        unsigned int new_height;
    public:
        WindowResizedEvent(unsigned int width, unsigned int height)
            : new_width(width), new_height(height) {}
        static AppEventType GetStaticType() { return AppEventType::WindowResized; }
        inline AppEventType GetType() override { return AppEventType::WindowResized; };
        inline unsigned int GetWidth() { return new_width; }
        inline unsigned int GetHeight() { return new_height; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "WindowResizedEvent (" << new_width << ", " << new_height << ")";
            return oss.str();
        }
    };

}
#endif