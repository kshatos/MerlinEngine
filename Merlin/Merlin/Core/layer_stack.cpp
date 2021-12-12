#include "Merlin/Core/layer_stack.hpp"


namespace Merlin
{
    void LayerStack::PushFront(std::shared_ptr<Layer> layer)
    {
        m_layers.push_front(layer);
        layer->OnAttach();
    }

    void LayerStack::PushBack(std::shared_ptr<Layer> layer)
    {
        m_layers.push_back(layer);
        layer->OnAttach();
    }

    void LayerStack::PopFront()
    {
        auto layer = m_layers.front();
        layer->OnDetatch();
        m_layers.pop_front();
    }

    void LayerStack::PopBack()
    {
        auto layer = m_layers.front();
        layer->OnDetatch();
        m_layers.pop_back();
    }

    void LayerStack::PopLayer(std::shared_ptr<Layer> layer)
    {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end())
            m_layers.erase(it);
    }

}
