#pragma once

#include <vector>

namespace sg
{
    class Layer;

    class LayerList
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        LayerList();

        LayerList(const LayerList& t_other) = delete;
        LayerList(LayerList&& t_other) noexcept = delete;
        LayerList& operator=(const LayerList& t_other) = delete;
        LayerList& operator=(LayerList&& t_other) noexcept = delete;

        ~LayerList();

        //-------------------------------------------------
        // Add / Remove
        //-------------------------------------------------

        void Add(Layer* t_layer);
        void Remove(Layer* t_layer);

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
    };
}
