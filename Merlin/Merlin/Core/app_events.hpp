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
        WindowResized,
        KeyPressed,
        KeyReleased
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

    // KEY EVENTS
    using KeyCode = uint16_t;

    class KeyPressedEvent : public AppEvent
    {
        KeyCode key;
        unsigned int repeat_count;
    public:
        KeyPressedEvent(KeyCode _key, int _repeat_count) : key(_key), repeat_count(_repeat_count) {}
        inline KeyCode GetKeyCode() { return key; }
        static AppEventType GetStaticType() { return AppEventType::KeyPressed; }
        inline AppEventType GetType() override { return AppEventType::KeyPressed; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "KeyPressedEvent: keycode " << key << ", repeat " << repeat_count;
            return oss.str();
        }
    };

    class KeyReleasedEvent : public AppEvent
    {
        KeyCode key;
    public:
        KeyReleasedEvent(KeyCode _key) : key(_key) {}
        inline KeyCode GetKeyCode() { return key; }
        static AppEventType GetStaticType() { return AppEventType::KeyReleased; }
        inline AppEventType GetType() override { return AppEventType::KeyReleased; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "KeyPressedEvent: keycode " << key;
            return oss.str();
        }
    };

}
#endif