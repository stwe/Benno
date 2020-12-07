#include "GameLayer.h"
#include "Game.h"
#include "Input.h"
#include "OpenGL.h"
#include "camera/OrthographicCamera.h"
#include "vendor/imgui/imgui.h"
#include "data/HousesJsonFile.h"
#include "file/PaletteFile.h"
#include "file/BshFile.h"
#include "file/GamFile.h"
#include "chunk/Island5.h"
#include "chunk/TileUtil.h"
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
    m_camera = std::make_unique<camera::OrthographicCamera>(this);
    m_camera->SetPosition(glm::vec2(0.0f, 0.0f));
    m_camera->SetCameraVelocity(1000.0f);

    m_housesJsonFile = std::make_shared<data::HousesJsonFile>("res/data/houses.json");

    m_paletteFile = std::make_unique<file::PaletteFile>(m_parentGame->GetFiles().GetColFile().path);
    m_paletteFile->ReadContentFromChunkData();

    auto stadtfld{ m_parentGame->GetFiles().GetBshFile(m_parentGame->gameOptions.currentZoom.GetZoomId(), file::BshFile::BshFileNameId::STADTFLD).value() };
    m_bshFile = std::make_shared<file::BshFile>(stadtfld.path, m_paletteFile->GetPalette());
    m_bshFile->ReadContentFromChunkData();

    m_gamFile = std::make_unique<file::GamFile>(
        "res/savegame/game01.gam",
        m_bshFile,
        m_housesJsonFile,
        m_parentGame->gameOptions.currentZoom
        );
    m_gamFile->ReadContentFromChunkData();

    m_meshRenderer = std::make_unique<renderer::MeshRenderer>();

    OpenGL::SetClearColor(0.4f, 0.4f, 0.7f);
}

void sg::GameLayer::OnDestruct()
{
}

void sg::GameLayer::OnUpdate()
{
    m_gamFile->Update(static_cast<int>(m_mapPosition.x), static_cast<int>(m_mapPosition.y));
}

void sg::GameLayer::OnRender()
{
    OpenGL::Clear();

    m_gamFile->Render(*m_camera, m_info, m_renderIslandAabbs);
}

void sg::GameLayer::OnGuiRender()
{
    ImGui::Begin("GameLayer Debug");

    ImGui::Text("Press the WASD keys to move the camera.");
    ImGui::Spacing();
    ImGui::Text("Camera x: %.1f", m_camera->GetPosition().x);
    ImGui::Text("Camera y: %.1f", m_camera->GetPosition().y);




    ////////////////////////////////////////////////////////////////////////////////////////////////

    const auto mx{ Input::GetInstance().GetMousePosition().x + m_camera->GetPosition().x };
    const auto my{ Input::GetInstance().GetMousePosition().y + m_camera->GetPosition().y };

    m_mapPosition = { chunk::TileUtil::ScreenToMap(
        mx,
        my,
        m_parentGame->gameOptions.currentZoom.GetTileWidth(),
        m_parentGame->gameOptions.currentZoom.GetTileHeight())
    };

    ImGui::Text("Map x: %d", static_cast<int>(m_mapPosition.x));
    ImGui::Text("Map y: %d", static_cast<int>(m_mapPosition.y));


    /*
    const auto screen{ chunk::TileUtil::MapToScreen(
        m_mapPosition.x,
        m_mapPosition.y,
        m_parentGame->gameOptions.currentZoom.GetXRaster(),
        m_parentGame->gameOptions.currentZoom.GetYRaster())
    };

    auto modelMatrix{ glm::mat4(1.0f) };
    modelMatrix = translate(modelMatrix, glm::vec3(
        screen.x,
        screen.y,
        0.0f)
    );
    modelMatrix = scale(modelMatrix, glm::vec3(
        m_parentGame->gameOptions.currentZoom.GetDefaultTileWidth(),
        m_parentGame->gameOptions.currentZoom.GetDefaultTileHeight(),
        1.0f)
    );

    if (m_mapPosition.x >= 0 && m_mapPosition.y >= 0)
    {
        m_meshRenderer->Render(modelMatrix, *m_camera, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    */

    ////////////////////////////////////////////////////////////////////////////////////////////////




    ImGui::Separator();

    ImGui::Text("Current zoom: %s", renderer::Zoom::ZoomToString(m_parentGame->gameOptions.currentZoom.GetZoomId()).c_str());

    ImGui::Separator();

    ImGui::Text("Render Islands: %d/%d", static_cast<int>(m_gamFile->GetIsland5List().size()) - m_info, static_cast<int>(m_gamFile->GetIsland5List().size()));
    ImGui::SameLine(200);
    ImGui::Checkbox("Toggle render Aabbs", &m_renderIslandAabbs);

    ImGui::Separator();

    // List Islands
    static auto selectedIslandNumber{ 0 };
    static chunk::Island5* selectedIsland5{ nullptr };

    ImGui::BeginChild("Islands", ImVec2(150, 0), true);

    for (const auto& island5 : m_gamFile->GetIsland5List())
    {
        auto label{ std::string("Island ") + std::to_string(island5->GetIsland5Data().islandNumber) };

        if (island5->selected)
        {
            selectedIslandNumber = island5->GetIsland5Data().islandNumber;
            selectedIsland5 = island5.get();
        }

        if (ImGui::Selectable(label.c_str(), selectedIslandNumber == island5->GetIsland5Data().islandNumber))
        {
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
    m_camera->OnUpdate(0.016f);
}
