#include "Layer.h"
#include "Game.h"
#include "SgAssert.h"
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::Layer::Layer(Game* t_parentGame, const std::string& t_name)
    : m_parentGame{ t_parentGame }
    , m_name{ t_name }
{
    SG_ASSERT(t_parentGame, "[Layer::Layer()] Null pointer.");

    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer {}.", m_name);
}

sg::Layer::~Layer()
{
    Log::SG_LOG_DEBUG("[Layer::~Layer()] Destruct Layer {}.", m_name);
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::Game* sg::Layer::GetParentGame() const noexcept
{
    return m_parentGame;
}

const std::string& sg::Layer::GetName() const noexcept
{
    return m_name;
}
