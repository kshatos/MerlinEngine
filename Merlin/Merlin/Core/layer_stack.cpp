#include "Merlin/Core/layer_stack.hpp"


namespace Merlin
{
    void LayerStack::PushFront(std::shared_ptr<Layer> layer)
    {
        layers.push_front(layer);
        layer->OnAttach();
    }

    void LayerStack::PushBack(std::shared_ptr<Layer> layer)
    {
        layers.push_back(layer);
        layer->OnAttach();
    }

    void LayerStack::PopFront()
    {
        auto layer = layers.front();
        layer->OnDetatch();
        layers.pop_front();
    }

    void LayerStack::PopBack()
    {
        auto layer = layers.front();
        layer->OnDetatch();
        layers.pop_back();
    }

    void LayerStack::PopLayer(std::shared_ptr<Layer> layer)
    {
        auto it = std::find(layers.begin(), layers.end(), layer);
        if (it != layers.end())
            layers.erase(it);
    }

}
