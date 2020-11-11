#include "LayerList.h"
#include "Layer.h"
#include "Log.h"
#include "SgAssert.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::LayerList::LayerList()
{
    Log::SG_LOG_DEBUG("[LayerList::LayerList()] Create LayerList.");
}

sg::LayerList::~LayerList()
{
    Log::SG_LOG_DEBUG("[LayerList::~LayerList()] Destruct LayerList.");

    for (auto* layer : m_layers)
    {
        if (layer)
        {
            layer->OnDestruct();
            delete layer;
        }
    }
}

//-------------------------------------------------
// Add / Remove
//-------------------------------------------------

void sg::LayerList::Add(Layer* t_layer)
{
    SG_ASSERT(t_layer, "[LayerList::Add()] Null pointer.");

    m_layers.emplace_back(t_layer);
}

void sg::LayerList::Remove(Layer* t_layer)
{
    SG_ASSERT(t_layer, "[LayerList::Remove()] Null pointer.");

    const auto it{ std::find(m_layers.begin(), m_layers.end(), t_layer) };
    if (it != m_layers.end())
    {
        t_layer->OnDestruct();
        delete t_layer;

        m_layers.erase(it);
    }
}
