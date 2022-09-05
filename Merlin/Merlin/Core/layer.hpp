#ifndef LAYER_HPP
#define LAYER_HPP
#include "Merlin/Core/app_events.hpp"

namespace Merlin
{
    class Application;

    class Layer
    {
    protected:
        Application* m_application;
    public:
        virtual ~Layer(){};
        virtual void OnAttach() = 0;
        virtual void OnDetatch() = 0;
        virtual void OnUpdate(float time_step) = 0;
        virtual void HandleEvent(AppEvent& app_event) = 0;

        friend class Application;
    };
}  // namespace Merlin

#endif