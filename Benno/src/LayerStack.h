#pragma once

#include <vector>

namespace sg
{
    class Layer;

    class LayerStack
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        LayerStack();

        LayerStack(const LayerStack& t_other) = delete;
        LayerStack(LayerStack&& t_other) noexcept = delete;
        LayerStack& operator=(const LayerStack& t_other) = delete;
        LayerStack& operator=(LayerStack&& t_other) noexcept = delete;

        ~LayerStack();

        //-------------------------------------------------
        // Push / Pop
        //-------------------------------------------------

        void PushLayer(Layer* t_layer);
        void PushOverlay(Layer* t_layer);
        void PopLayer(Layer* t_layer);
        void PopOverlay(Layer* t_layer);

        //-------------------------------------------------
        // Iterator
        //-------------------------------------------------

        std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_layers.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

        [[nodiscard]] std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
        [[nodiscard]] std::vector<Layer*>::const_iterator end() const { return m_layers.end(); }
        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }

    protected:

    private:
        std::vector<Layer*> m_layers;
        uint32_t m_layerInsertIndex{ 0 };
    };
}
