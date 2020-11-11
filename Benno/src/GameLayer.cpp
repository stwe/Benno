#include "GameLayer.h"
#include "Game.h"
#include "OpenGL.h"
#include "vendor/imgui/imgui.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::GameLayer::GameLayer(Game* t_parentGame, const std::string& t_name)
    : Layer(t_parentGame, t_name)
{
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::GameLayer::OnCreate()
{
    OpenGL::SetClearColor(0.4f, 0.4f, 0.7f);
}

void sg::GameLayer::OnDestruct()
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
    ImGui::Begin("Debug");

    ImGui::End();
}

void sg::GameLayer::OnEvent()
{
}
