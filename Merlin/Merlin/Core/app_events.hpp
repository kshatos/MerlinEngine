#ifndef APP_EVENTS_HPP
#define APP_EVENTS_HPP
#include <string>


namespace Merlin
{
    enum AppEventType
    {
        None = 0
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

}
#endif