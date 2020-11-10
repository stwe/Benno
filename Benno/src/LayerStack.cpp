#include "LayerStack.h"
#include "Layer.h"
#include "Log.h"
#include "SgAssert.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::LayerStack::LayerStack()
{
    Log::SG_LOG_DEBUG("[LayerStack::LayerStack()] Create LayerStack.");
}

sg::LayerStack::~LayerStack()
{
    Log::SG_LOG_DEBUG("[LayerStack::~LayerStack()] Destruct LayerStack.");

    for (auto* layer : m_layers)
    {
        layer->OnDetach();
        delete layer;
    }
}

//-------------------------------------------------
// Push / Pop
//-------------------------------------------------

void sg::LayerStack::PushLayer(Layer* t_layer)
{
    SG_ASSERT(t_layer, "[LayerStack::PushLayer()] Null pointer.");

    m_layers.emplace(m_layers.begin() + m_layerInsertIndex, t_layer);
    m_layerInsertIndex++;
}

void sg::LayerStack::PushOverlay(Layer* t_layer)
{
    SG_ASSERT(t_layer, "[LayerStack::PushOverlay()] Null pointer.");

    m_layers.emplace_back(t_layer);
}

void sg::LayerStack::PopLayer(Layer* t_layer)
{
    SG_ASSERT(t_layer, "[LayerStack::PopLayer()] Null pointer.");

    const auto it{ std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, t_layer) };
    if (it != m_layers.begin() + m_layerInsertIndex)
    {
        t_layer->OnDetach();
        m_layers.erase(it);
        m_layerInsertIndex--;
    }
}

void sg::LayerStack::PopOverlay(Layer* t_layer)
{
    SG_ASSERT(t_layer, "[LayerStack::PopOverlay()] Null pointer.");

    const auto it{ std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), t_layer) };
    if (it != m_layers.end())
    {
        t_layer->OnDetach();
        m_layers.erase(it);
    }
}
