#include "GameLayer.h"
#include "Game.h"
#include "OpenGL.h"
#include "Window.h"
#include "vendor/imgui/imgui.h"
#include "renderer/MeshRenderer.h"

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
    m_paletteFile = std::make_unique<file::PaletteFile>(m_parentGame->GetFiles().GetColFile().path);
    m_paletteFile->ReadContentFromChunkData();

    auto stadtfld{ m_parentGame->GetFiles().GetBshFile(m_parentGame->gameOptions.currentZoomId, file::BshFile::BshFileNameId::STADTFLD).value() };
    m_bshFile = std::make_unique<file::BshFile>(stadtfld.path, m_paletteFile->GetPalette());
    m_bshFile->ReadContentFromChunkData();

    m_renderer = std::make_unique<renderer::MeshRenderer>();

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

    // Galgen
    const auto& texture{ m_bshFile->GetBshTexture(m_bshIndex) };
    m_renderer->Render(
        250, 250,
        texture,
        m_parentGame->GetWindow().GetOrthographicProjectionMatrix()
    );

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
