#ifndef LAYER_STACK_HPP
#define LAYER_STACK_HPP
#include <deque>
#include <vector>

#include "Merlin/Core/layer.hpp"

namespace Merlin
{
    class LayerStack
    {
        std::deque<std::shared_ptr<Layer>> m_layers;

    public:
        void PushFront(std::shared_ptr<Layer> layer);
        void PushBack(std::shared_ptr<Layer> layer);
        void PopFront();
        void PopBack();
        void PopLayer(std::shared_ptr<Layer> layer);
        inline std::deque<std::shared_ptr<Layer>>::iterator begin()
        {
            return m_layers.begin();
        }
        inline std::deque<std::shared_ptr<Layer>>::iterator end()
        {
            return m_layers.end();
        }
        inline std::deque<std::shared_ptr<Layer>>::const_iterator begin() const
        {
            return m_layers.begin();
        }
        inline std::deque<std::shared_ptr<Layer>>::const_iterator end() const
        {
            return m_layers.end();
        }
    };
}  // namespace Merlin

#endif