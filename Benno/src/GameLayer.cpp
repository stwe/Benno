#include "GameLayer.h"
#include "Game.h"
#include "OpenGL.h"
#include "Window.h"
#include "vendor/imgui/imgui.h"
#include "data/HousesJsonFile.h"
#include "file/PaletteFile.h"
#include "file/BshFile.h"
#include "file/GamFile.h"
#include "chunk/Island5.h"

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
    m_housesJsonFile = std::make_shared<data::HousesJsonFile>("res/data/houses.json");

    m_paletteFile = std::make_unique<file::PaletteFile>(m_parentGame->GetFiles().GetColFile().path);
    m_paletteFile->ReadContentFromChunkData();

    auto stadtfld{ m_parentGame->GetFiles().GetBshFile(m_parentGame->gameOptions.currentZoom.GetZoomId(), file::BshFile::BshFileNameId::STADTFLD).value() };
    m_bshFile = std::make_shared<file::BshFile>(stadtfld.path, m_paletteFile->GetPalette());
    m_bshFile->ReadContentFromChunkData();

    m_gamFile = std::make_unique<file::GamFile>("res/savegame/game01.gam", m_bshFile, m_housesJsonFile, m_parentGame->gameOptions.currentZoom);
    m_gamFile->ReadContentFromChunkData();

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

    m_gamFile->Render(m_parentGame->GetWindow().GetOrthographicProjectionMatrix());

    OpenGL::DisableBlending();
}

void sg::GameLayer::OnGuiRender()
{
    ImGui::Begin("GameLayer Debug");

    // List Islands
    static auto selectedIslandNumber{ 0 };
    static chunk::Island5* selectedIsland5{ nullptr };

    ImGui::BeginChild("Islands", ImVec2(150, 0), true);

    for (const auto& island5 : m_gamFile->GetIsland5List())
    {
        auto label{ std::string("Island ") + std::to_string(island5->GetIsland5Data().islandNumber) };
        if (ImGui::Selectable(label.c_str(), selectedIslandNumber == island5->GetIsland5Data().islandNumber))
        {
            selectedIslandNumber = island5->GetIsland5Data().islandNumber;
            selectedIsland5 = island5.get();
        }
    }

    ImGui::EndChild();

    ImGui::SameLine();

    // Island5 data view
    ImGui::BeginGroup();
    ImGui::BeginChild("Island5 data view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    ImGui::Text("Selected Island: %d", selectedIslandNumber);
    ImGui::Separator();

    if (selectedIsland5)
    {
        ImGui::Text("Width: %d", selectedIsland5->GetIsland5Data().width);
        ImGui::Text("Height: %d", selectedIsland5->GetIsland5Data().height);
        ImGui::Text("Position x: %d", selectedIsland5->GetIsland5Data().posx);
        ImGui::Text("Position y: %d", selectedIsland5->GetIsland5Data().posy);
        ImGui::Text("Modified: %s", selectedIsland5->GetIsland5Data().modifiedFlag == 1 ? "yes" : "no");
        ImGui::Text("Fertility: %s", chunk::Island::FertilityToString(selectedIsland5->GetIsland5Data().fertility).c_str());
        ImGui::Text("Size: %s", chunk::Island::SizeToString(selectedIsland5->GetIsland5Data().size).c_str());
        ImGui::Text("Climate: %s", chunk::Island::ClimateToString(selectedIsland5->GetIsland5Data().climate).c_str());
    }

    ImGui::EndChild();
    ImGui::EndGroup();

    ImGui::End();
}

void sg::GameLayer::OnSdlEvent(const SDL_Event& t_event)
{
}

void sg::GameLayer::OnInput()
{
}
