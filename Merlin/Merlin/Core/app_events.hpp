#ifndef APP_EVENTS_HPP
#define APP_EVENTS_HPP
#include <string>
#include <sstream>
#include <functional>
#include "Merlin/Core/input_codes.hpp"


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
        bool m_was_handled = false;

        virtual ~AppEvent() {}
        virtual AppEventType GetType() = 0;
        virtual std::string ToString() = 0;

        template<typename T>
        void Dispatch(std::function<bool(T&)> callback)
        {
            if (GetType() == T::GetStaticType())
                m_was_handled |= callback(static_cast<T&>(*this));
        }
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
        unsigned int m_new_width;
        unsigned int m_new_height;
    public:
        WindowResizedEvent(unsigned int new_width, unsigned int new_height)
            : m_new_width(new_width), m_new_height(new_height) {}
        static AppEventType GetStaticType() { return AppEventType::WindowResized; }
        inline AppEventType GetType() override { return AppEventType::WindowResized; };
        inline unsigned int GetWidth() { return m_new_width; }
        inline unsigned int GetHeight() { return m_new_height; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "WindowResizedEvent (" << m_new_width << ", " << m_new_height << ")";
            return oss.str();
        }
    };

    // KEY EVENTS
    class KeyPressedEvent : public AppEvent
    {
        KeyCode m_key;
        unsigned int m_repeat_count;
    public:
        KeyPressedEvent(KeyCode key, int repeat_count) : m_key(key), m_repeat_count(repeat_count) {}
        inline KeyCode GetKeyCode() { return m_key; }
        static AppEventType GetStaticType() { return AppEventType::KeyPressed; }
        inline AppEventType GetType() override { return AppEventType::KeyPressed; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "KeyPressedEvent: keycode " << m_key << ", repeat " << m_repeat_count;
            return oss.str();
        }
    };

    class KeyReleasedEvent : public AppEvent
    {
        KeyCode m_key;
    public:
        KeyReleasedEvent(KeyCode key) : m_key(key) {}
        inline KeyCode GetKeyCode() { return m_key; }
        static AppEventType GetStaticType() { return AppEventType::KeyReleased; }
        inline AppEventType GetType() override { return AppEventType::KeyReleased; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "KeyPressedEvent: keycode " << m_key;
            return oss.str();
        }
    };

    // MOUSE EVENTS

    class MouseMovedEvent : public AppEvent
    {
        float m_x_position;
        float m_y_position;
    public:
        MouseMovedEvent(float x_position, float y_position) :
            m_x_position(x_position),
            m_y_position(y_position) {}
        static AppEventType GetStaticType() { return AppEventType::MouseMoved; }
        inline AppEventType GetType() override { return AppEventType::MouseMoved; }
        inline float GetXPosition() { return m_x_position; }
        inline float GetYPosition() { return m_y_position; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "MouseMovedEvent (" << m_x_position << ", " << m_y_position << ")";
            return oss.str();
        }
    };

    class MouseButtonPressedEvent : public AppEvent
    {
        MouseCode m_button;
    public:
        MouseButtonPressedEvent(MouseCode button) : m_button(button) {}
        static AppEventType GetStaticType() { return AppEventType::MouseButtonPressed; }
        inline AppEventType GetType() override { return AppEventType::MouseButtonPressed; }
        inline MouseCode GetButton() { return m_button; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "MouseButtonPressedEvent: buttoncode " << m_button;
            return oss.str();
        }
    };

    class MouseButtonReleasedEvent : public AppEvent
    {
        MouseCode m_button;
    public:
        MouseButtonReleasedEvent(MouseCode button) : m_button(button) {}
        static AppEventType GetStaticType() { return AppEventType::MouseButtonReleased; }
        inline AppEventType GetType() override { return AppEventType::MouseButtonReleased; }
        inline MouseCode GetButton() { return m_button; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "MouseButtonReleasedEvent: buttoncode " << m_button;
            return oss.str();
        }
    };

    class MouseScrolledEvent : public AppEvent
    {
        float m_x_scroll_delta;
        float m_y_scroll_delta;
    public:
        MouseScrolledEvent(float x_scroll, float y_scroll) :
            m_x_scroll_delta(x_scroll),
            m_y_scroll_delta(y_scroll) {}
        static AppEventType GetStaticType() { return AppEventType::MouseScrolled; }
        inline AppEventType GetType() override { return AppEventType::MouseScrolled; }
        inline float GetXScrollDelta() { return m_x_scroll_delta; }
        inline float GetYScrollDelta() { return m_y_scroll_delta; }
        std::string ToString() override
        {
            std::ostringstream oss;
            oss << "MouseScrolledEvent (" << m_x_scroll_delta << ", " << m_y_scroll_delta << ")";
            return oss.str();
        }
    };

}
#endif