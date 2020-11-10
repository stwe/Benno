#include "GameLayer.h"
#include "Game.h"
#include "OpenGL.h"
#include "vendor/imgui/imgui.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::GameLayer::GameLayer(Game* t_parentGame)
    : Layer(t_parentGame)
{
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::GameLayer::OnAttach()
{
    OpenGL::SetClearColor(0.4f, 0.4f, 0.7f);
}

void sg::GameLayer::OnDetach()
{
}

void sg::GameLayer::OnUpdate()
{
}

void sg::GameLayer::OnRender()
{
    OpenGL::Clear();
    OpenGL::EnableAlphaBlending();



    OpenGL::DisableBlending();
}

void sg::GameLayer::OnGuiRender()
{
    ImGui::Begin("Settings");

    ImGui::End();
}

void sg::GameLayer::OnEvent()
{
}
