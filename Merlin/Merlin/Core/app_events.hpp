#ifndef APP_EVENTS_HPP
#define APP_EVENTS_HPP
#include <string>
#include <sstream>
#include <functional>


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
        bool was_handled = false;
    public:
        virtual ~AppEvent() {}
        virtual AppEventType GetType() = 0;
        virtual std::string ToString() = 0;

        template<typename T>
        static void Dispatch(
            AppEvent& app_event,
            std::function<bool(T&)> callback)
        {
            if (app_event.GetType() == T::GetStaticType())
            {
                app_event.was_handled |= callback(static_cast<T&>(app_event));
            }
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
    enum Key : KeyCode
    {
        // From glfw
        SPACE = 32,
        APOSTROPHE = 39,
        COMMA = 44,
        MINUS = 45,
        PERIOD = 46,
        SLASH = 47,
        DIGIT_0 = 48,
        DIGIT_1 = 49,
        DIGIT_2 = 50,
        DIGIT_3 = 51,
        DIGIT_4 = 52,
        DIGIT_5 = 53,
        DIGIT_6 = 54,
        DIGIT_7 = 55,
        DIGIT_8 = 56,
        DIGIT_9 = 57,
        SEMICOLON = 59,
        EQUAL = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LEFT_BRACKET = 91,
        BACKSLASH = 92,
        RIGHT_BRACKET = 93,
        GRAVE_ACCENT = 96,
        WORLD_1 = 161,
        WORLD_2 = 162,
        ESCAPE = 256,
        ENTER = 257,
        TAB = 258,
        BACKSPACE = 259,
        INSERT = 260,
        DELETE = 261,
        RIGHT = 262,
        LEFT = 263,
        DOWN = 264,
        UP = 265,
        PAGE_UP = 266,
        PAGE_DOWN = 267,
        HOME = 268,
        END = 269,
        CAPS_LOCK = 280,
        SCROLL_LOCK = 281,
        NUM_LOCK = 282,
        PRINT_SCREEN = 283,
        PAUSE = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KP_0 = 320,
        KP_1 = 321,
        KP_2 = 322,
        KP_3 = 323,
        KP_4 = 324,
        KP_5 = 325,
        KP_6 = 326,
        KP_7 = 327,
        KP_8 = 328,
        KP_9 = 329,
        KP_DECIMAL = 330,
        KP_DIVIDE = 331,
        KP_MULTIPLY = 332,
        KP_SUBTRACT = 333,
        KP_ADD = 334,
        KP_ENTER = 335,
        KP_EQUAL = 336,
        LEFT_SHIFT = 340,
        LEFT_CONTROL = 341,
        LEFT_ALT = 342,
        LEFT_SUPER = 343,
        RIGHT_SHIFT = 344,
        RIGHT_CONTROL = 345,
        RIGHT_ALT = 346,
        RIGHT_SUPER = 347,
        MENU = 348,
        LAST = MENU,
    };

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
    enum MouseButton : MouseCode
    {
        BUTTON_1 = 0,
        BUTTON_2 = 1,
        BUTTON_3 = 2,
        BUTTON_4 = 3,
        BUTTON_5 = 4,
        BUTTON_6 = 5,
        BUTTON_7 = 6,
        BUTTON_8 = 7,
        BUTTON_LAST = BUTTON_8,
        BUTTON_LEFT = BUTTON_1,
        BUTTON_RIGHT = BUTTON_2,
        BUTTON_MIDDLE = BUTTON_3,
    };

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