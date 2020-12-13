#include <magic_enum.hpp>
#include "GameLayer.h"
#include "Game.h"
#include "Input.h"
#include "Log.h"
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
#include "gl/ShaderManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::GameLayer::GameLayer(Game* t_parentGame, const std::string& t_name)
    : Layer(t_parentGame, t_name)
{
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::renderer::Zoom& sg::GameLayer::GetCurrentZoom() const noexcept
{
    return m_currentZoom;
}

std::shared_ptr<sg::renderer::MeshRenderer> sg::GameLayer::GetMeshRenderer() const noexcept
{
    return m_meshRenderer;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::GameLayer::SetCurrentZoom(const renderer::Zoom::ZoomId t_zoomId)
{
    m_currentZoom = m_parentGame->GetZoomFactory().GetZooms().at(t_zoomId);
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::GameLayer::OnCreate()
{
    SetCurrentZoom(m_parentGame->gameOptions.initialZoomId);

    m_camera = std::make_unique<camera::OrthographicCamera>(this);
    m_camera->SetPosition(glm::vec2(0.0f, 0.0f));
    m_camera->SetCameraVelocity(1000.0f);

    m_housesJsonFile = std::make_shared<data::HousesJsonFile>(m_parentGame->gameOptions.housesJsonFilePath);

    m_paletteFile = std::make_unique<file::PaletteFile>(m_parentGame->GetFiles().GetColFile().path);
    m_paletteFile->ReadContentFromChunkData();

    for (const auto& zoomId : magic_enum::enum_values<renderer::Zoom::ZoomId>())
    {
        auto stadtfldFile{ std::make_unique<file::BshFile>(
            m_parentGame->GetFiles().GetBshFile(zoomId, file::BshFile::BshFileNameId::STADTFLD).value().path, m_paletteFile->GetPalette())
        };
        stadtfldFile->ReadContentFromChunkData();
        m_stadtfldFiles.emplace(zoomId, std::move(stadtfldFile));

        auto gamFile{ std::make_unique<file::GamFile>(
            this,
            "res/savegame/game01.gam",
            m_stadtfldFiles.at(zoomId),
            m_housesJsonFile,
            m_parentGame->GetZoomFactory().GetZooms().at(zoomId)
            )
        };
        gamFile->ReadContentFromChunkData();
        m_gamFiles.emplace(zoomId, std::move(gamFile));
    }

    m_meshRenderer = std::make_shared<renderer::MeshRenderer>(m_parentGame->GetShaderManager());

    OpenGL::SetClearColor(0.4f, 0.4f, 0.7f);
}

void sg::GameLayer::OnDestruct()
{
}

void sg::GameLayer::OnUpdate()
{
    m_gamFiles.at(m_currentZoom.GetZoomId())->Update(static_cast<int>(m_mapPosition.x), static_cast<int>(m_mapPosition.y));
}

void sg::GameLayer::OnRender()
{
    OpenGL::Clear();

    m_gamFiles.at(m_currentZoom.GetZoomId())->Render(*m_camera, m_info, m_renderIslandAabbs);
}

void sg::GameLayer::OnGuiRender()
{
    ImGui::Begin("GameLayer Debug");

    ImGui::Text("Press the WASD or arrow keys to move the camera.");
    ImGui::Spacing();
    ImGui::Text("Camera x: %.1f", m_camera->GetPosition().x);
    ImGui::Text("Camera y: %.1f", m_camera->GetPosition().y);

    //ImGui::SliderFloat("Camera velocity: ", &m_camera->GetCameraVelocity(), 1000.0f, 10000.0f, "%.1f");




    ////////////////////////////////////////////////////////////////////////////////////////////////

    // get map position

    const auto mx{ Input::GetInstance().GetMousePosition().x + m_camera->GetPosition().x };
    const auto my{ Input::GetInstance().GetMousePosition().y + m_camera->GetPosition().y };

    m_mapPosition = { chunk::TileUtil::ScreenToMap(
        mx,
        my,
        m_currentZoom.GetTileWidth(),
        m_currentZoom.GetTileHeight())
    };

    m_onIsland = false;

    auto& gam{ m_gamFiles.at(m_currentZoom.GetZoomId()) };
    if (gam->IsIslandOnPosition(m_mapPosition.x, m_mapPosition.y, gam->GetIsland5List()))
    {
        m_mapPosition = { chunk::TileUtil::IslandScreenToMap(
            mx,
            my,
            m_currentZoom.GetTileWidth(),
            m_currentZoom.GetTileHeight())
        };

        m_onIsland = true;
    }

    ImGui::Separator();

    m_onIsland ? ImGui::Text("Island") : ImGui::Text("Deep water");
    ImGui::Text("Map x: %d", static_cast<int>(m_mapPosition.x));
    ImGui::Text("Map y: %d", static_cast<int>(m_mapPosition.y));

    // show map position

    auto modelMatrix{ glm::mat4(1.0f) };
    modelMatrix = translate(modelMatrix, glm::vec3(
        mx - static_cast<float>(m_currentZoom.GetXRaster()),
        my - static_cast<float>(m_currentZoom.GetYRaster()),
        0.0f)
    );
    modelMatrix = scale(modelMatrix, glm::vec3(
        m_currentZoom.GetDefaultTileWidth(),
        m_currentZoom.GetDefaultTileHeight(),
        1.0f)
    );

    if (m_mapPosition.x >= 0 && m_mapPosition.y >= 0)
    {
        m_meshRenderer->Render(modelMatrix, *m_camera, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////




    ImGui::Separator();

    const auto zoomId{ magic_enum::enum_name(m_currentZoom.GetZoomId()) };
    ImGui::Text("Current zoom: %s", std::string(zoomId).c_str());
    ImGui::SameLine(200);

    static const auto* itemCurrent{ renderer::Zoom::zoomMenuItems[magic_enum::enum_integer(m_currentZoom.GetZoomId())] };

    if (ImGui::BeginCombo("Change zoom", itemCurrent, 0))
    {
        for (auto n{ 0 }; n < IM_ARRAYSIZE(renderer::Zoom::zoomMenuItems); ++n)
        {
            const auto isSelected = (itemCurrent == renderer::Zoom::zoomMenuItems[n]);

            if (ImGui::Selectable(renderer::Zoom::zoomMenuItems[n], isSelected))
            {
                itemCurrent = renderer::Zoom::zoomMenuItems[n];

                auto current{ magic_enum::enum_cast<renderer::Zoom::ZoomId>(std::string(itemCurrent)) };
                if (current.has_value())
                {
                    SetCurrentZoom(current.value());
                }
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    ImGui::Text("Render Islands: %d/%d", static_cast<int>(m_gamFiles.at(m_currentZoom.GetZoomId())->GetIsland5List().size()) - m_info, static_cast<int>(m_gamFiles.at(m_currentZoom.GetZoomId())->GetIsland5List().size()));
    ImGui::SameLine(200);
    ImGui::Checkbox("Toggle render Aabbs", &m_renderIslandAabbs);

    ImGui::Separator();

    // List Islands
    static auto selectedIslandNumber{ 0 };
    static chunk::Island5* selectedIsland5{ nullptr };

    ImGui::BeginChild("Islands", ImVec2(150, 0), true);

    for (const auto& island5 : m_gamFiles.at(m_currentZoom.GetZoomId())->GetIsland5List())
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
