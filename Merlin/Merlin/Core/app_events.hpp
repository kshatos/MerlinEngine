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
        KeyReleased,
        MouseMoved,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseScrolled
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

    // MOUSE EVENTS
    using MouseCode = uint16_t;

    class MouseMovedEvent : public AppEvent
    {
        float x_position;
        float y_position;
    public:
        MouseMovedEvent(float x, float y) : x_position(x), y_position(y) {}
        static AppEventType GetStaticType() { return AppEventType::MouseMoved; }
        inline AppEventType GetType() override { return AppEventType::MouseMoved; }
        inline float GetX() { return x_position; }
        inline float GetY() { return y_position; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "MouseMovedEvent (" << x_position << ", " << y_position << ")";
            return oss.str();
        }
    };

    class MouseButtonPressedEvent : public AppEvent
    {
        MouseCode button;
    public:
        MouseButtonPressedEvent(MouseCode _button) : button(_button) {}
        static AppEventType GetStaticType() { return AppEventType::MouseButtonPressed; }
        inline AppEventType GetType() override { return AppEventType::MouseButtonPressed; }
        inline MouseCode GetButton() { return button; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "MouseButtonPressedEvent: buttoncode " << button;
            return oss.str();
        }
    };

    class MouseButtonReleasedEvent : public AppEvent
    {
        MouseCode button;
    public:
        MouseButtonReleasedEvent(MouseCode _button) : button(_button) {}
        static AppEventType GetStaticType() { return AppEventType::MouseButtonReleased; }
        inline AppEventType GetType() override { return AppEventType::MouseButtonReleased; }
        inline MouseCode GetButton() { return button; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "MouseButtonReleasedEvent: buttoncode " << button;
            return oss.str();
        }
    };

    class MouseScrolledEvent : public AppEvent
    {
        float x_scroll;
        float y_scroll;
    public:
        MouseScrolledEvent(float _x_scroll, float _y_scroll) : x_scroll(_x_scroll), y_scroll(_y_scroll) {}
        static AppEventType GetStaticType() { return AppEventType::MouseScrolled; }
        inline AppEventType GetType() override { return AppEventType::MouseScrolled; }
        inline float GetXScroll() { return x_scroll; }
        inline float GetYScroll() { return y_scroll; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "MouseScrolledEvent (" << x_scroll << ", " << y_scroll << ")";
            return oss.str();
        }
    };

}
#endif