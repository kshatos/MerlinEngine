#ifndef LAYER_STACK_HPP
#define LAYER_STACK_HPP
#include <vector>
#include <deque>
#include"Merlin/Core/layer.hpp"


namespace Merlin
{
    class LayerStack
    {
        std::deque<std::shared_ptr<Layer>> layers;
    public:
        void PushFront(std::shared_ptr<Layer> layer);
        void PushBack(std::shared_ptr<Layer> layer);
        void PopFront();
        void PopBack();
        void PopLayer(std::shared_ptr<Layer> layer);
        inline std::deque<std::shared_ptr<Layer>>::iterator begin() { return layers.begin(); }
        inline std::deque<std::shared_ptr<Layer>>::iterator end() { return layers.end(); }
        inline std::deque<std::shared_ptr<Layer>>::const_iterator begin() const { return layers.begin(); }
        inline std::deque<std::shared_ptr<Layer>>::const_iterator end() const { return layers.end(); }
    };
}

#endif