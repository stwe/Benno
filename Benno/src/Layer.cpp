#include "Layer.h"
#include "Game.h"
#include "SgAssert.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::Layer::Layer(Game* t_parentGame)
    : m_parentGame{ t_parentGame }
{
    SG_ASSERT(t_parentGame, "[Layer::Layer()] Null pointer.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::Game* sg::Layer::GetParentGame() const noexcept
{
    return m_parentGame;
}
