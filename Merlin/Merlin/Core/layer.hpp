#ifndef LAYER_HPP
#define LAYER_HPP
#include "Merlin/Core/app_events.hpp"


namespace Merlin
{
    class Layer
    {
    public:
        virtual ~Layer() {};
        virtual void OnAttach() = 0;
        virtual void OnDetatch() = 0;
        virtual void OnUpdate() = 0;
        virtual void HandleEvent(AppEvent& app_event) = 0;
    };
}

#endif