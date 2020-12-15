#include <SDL_mouse.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "GamFile.h"
#include "BshFile.h"
#include "BshTexture.h"
#include "Input.h"
#include "Log.h"
#include "SgAssert.h"
#include "GameLayer.h"
#include "data/HousesJsonFile.h"
#include "chunk/Island5.h"
#include "chunk/IslandHouse.h"
#include "chunk/Chunk.h"
#include "chunk/TileUtil.h"
#include "renderer/DeepWaterRenderer.h"
#include "renderer/IslandModel.h"
#include "renderer/MeshRenderer.h"
#include "camera/OrthographicCamera.h"
#include "physics/Aabb.h"
#include "vendor/imgui/imgui.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::file::GamFile::GamFile(
    GameLayer* t_parentLayer,
    const std::string& t_filePath,
    std::shared_ptr<BshFile> t_stadtfldFile,
    std::shared_ptr<data::HousesJsonFile> t_housesJsonFile,
    const renderer::Zoom& t_zoom
)
    : BinaryFile(t_filePath)
    , m_parentLayer{ t_parentLayer }
    , m_stadtfldFile{ std::move(t_stadtfldFile) }
    , m_housesJsonFile{ std::move(t_housesJsonFile) }
    , m_zoom{ t_zoom }
{
    Log::SG_LOG_DEBUG("[GamFile::GamFile()] Creates GamFile object from file {}.", t_filePath);

    SG_ASSERT(m_parentLayer, "[GamFile::GamFile()] Null pointer.");
}

sg::file::GamFile::~GamFile()
{
    Log::SG_LOG_DEBUG("[GamFile::~GamFile()] Destruct GamFile.");
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

const std::vector<std::unique_ptr<sg::chunk::Island5>>& sg::file::GamFile::GetIsland5List() const noexcept
{
    return m_island5List;
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void sg::file::GamFile::Render(
    camera::OrthographicCamera& t_camera,
    int& t_info,
    const bool t_renderIslandAabbs
)
{
    m_deepWaterRenderer->Render(t_camera, false);

    t_info = 0;
    for (const auto& islandModel : m_islandModels)
    {
        auto intersect{ false };
        if (physics::Aabb::AabbVsAabb(t_camera.GetCurrentAabb(), islandModel->GetAabb()))
        {
            if (!ImGui::GetIO().WantCaptureMouse)
            {
                const auto mx{ Input::GetInstance().GetMousePosition().x };
                const auto my{ Input::GetInstance().GetMousePosition().y };

                if (physics::Aabb::PointVsAabb(glm::vec2(mx + t_camera.GetPosition().x, my + t_camera.GetPosition().y), islandModel->GetAabb()))
                {
                    for (const auto& island5 : m_island5List)
                    {
                        island5->selected = false;
                    }

                    intersect = true;
                    islandModel->GetParentIsland()->selected = true;
                }
            }

            islandModel->Render(t_camera);
        }
        else
        {
            t_info++;
        }

        if (t_renderIslandAabbs)
        {
            auto aabbModelMatrix{ islandModel->GetAabb().GetModelMatrix() };
            if (intersect)
            {
                m_parentLayer->GetMeshRenderer()->Render(aabbModelMatrix, t_camera, glm::vec3(1.0f, 1.0f, 0.0f));
            }
            else
            {
                m_parentLayer->GetMeshRenderer()->Render(aabbModelMatrix, t_camera, glm::vec3(1.0f, 0.0f, 0.0f));
            }
        }
    }
}

void sg::file::GamFile::Update(const int t_mapX, const int t_mapY)
{
    if (Input::GetInstance().IsKeyDown(SDL_BUTTON_LEFT))
    {
        auto index{ DeepWaterTileInVbo(t_mapX, t_mapY) };

        if (index > NO_DEEP_WATER)
        {
            m_deepWaterRenderer->UpdateIntensity(index, chunk::TileUtil::SELECTED);
        }
        else
        {
            for (const auto& islandModel : m_islandModels)
            {
                index = islandModel->IslandTileInVbo(t_mapX, t_mapY);
                if (index > NO_ISLAND)
                {
                    islandModel->UpdateIntensity(index, chunk::TileUtil::SELECTED);
                }
            }
        }
    }
}

//-------------------------------------------------
// BinaryFile Interface
//-------------------------------------------------

void sg::file::GamFile::ReadContentFromChunkData()
{
    Log::SG_LOG_DEBUG("[GamFile::ReadContentFromChunkData()] Start reading savegame data from Chunks...");

    for (const auto& chunk : GetChunks())
    {
        const auto* id{ chunk->GetId() };

        if (id == std::string("INSEL5"))
        {
            m_island5List.emplace_back(std::make_unique<chunk::Island5>(chunk->GetData(), m_housesJsonFile));
        }

        if (id == std::string("INSELHAUS"))
        {
            auto* currentIsland5{ m_island5List.back().get() };
            currentIsland5->AddIslandHouse(std::make_unique<chunk::IslandHouse>(chunk->GetData(), currentIsland5, m_housesJsonFile));
        }
    }

    InitIsland5Layer();
    InitDeepWaterArea();
    InitIslandsArea();

    Log::SG_LOG_DEBUG("[GamFile::ReadContentFromChunkData()] Savegame data read successfully.");
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

sg::chunk::Island5* sg::file::GamFile::IsIslandOnPosition(const int t_x, const int t_y, const std::vector<std::unique_ptr<chunk::Island5>>& t_island5List)
{
    for (const auto& island5 : t_island5List)
    {
        if (t_x >= island5->GetIsland5Data().posx &&
            t_y >= island5->GetIsland5Data().posy &&
            t_x < island5->GetIsland5Data().posx + island5->GetIsland5Data().width &&
            t_y < island5->GetIsland5Data().posy + island5->GetIsland5Data().height
            )
        {
            return island5.get();
        }
    }

    return nullptr;
}

//-------------------------------------------------
// Island5 Layer
//-------------------------------------------------

void sg::file::GamFile::InitIsland5Layer()
{
    Log::SG_LOG_DEBUG("[GamFile::InitIsland5Layer()] Initialize layer.");

    for (auto& island5 : m_island5List)
    {
        island5->InitLayer();
    }
}

//-------------------------------------------------
// Deep water area
//-------------------------------------------------

void sg::file::GamFile::InitDeepWaterArea()
{
    Log::SG_LOG_DEBUG("[GamFile::InitDeepWaterArea()] Create data for the Deep Water Renderer.");

    std::vector<chunk::TileGraphic> deepWaterGraphicTiles;
    std::vector<glm::mat4> deepWaterModelMatrices;
    std::vector<int> deepWaterTextureBuffer;
    std::vector<glm::vec3> intensityBuffer;

    CreateDeepWaterGraphicTiles(deepWaterGraphicTiles);

    for (const auto& tile : deepWaterGraphicTiles)
    {
        deepWaterModelMatrices.push_back(tile.GetModelMatrix());
        deepWaterTextureBuffer.push_back(tile.tileGfxInfo.gfxIndex - renderer::DeepWaterRenderer::START_GFX_INDEX);
    }

    intensityBuffer.resize(deepWaterGraphicTiles.size(), chunk::TileUtil::NOT_SELECTED);

    m_deepWaterRenderer = std::make_unique<renderer::DeepWaterRenderer>(
        m_parentLayer->GetParentGame()->GetShaderManager(),
        m_stadtfldFile,
        std::move(deepWaterModelMatrices),
        std::move(deepWaterTextureBuffer),
        std::move(intensityBuffer)
        );
    m_deepWaterRenderer->Init(m_zoom);
}

void sg::file::GamFile::CreateDeepWaterGraphicTiles(std::vector<chunk::TileGraphic>& t_graphicTiles)
{
    constexpr auto waterId{ 1201 };

    m_deepWaterIndex.resize(Game::WORLD_HEIGHT * Game::WORLD_WIDTH, NO_DEEP_WATER);

    for (auto y{ 0 }; y < Game::WORLD_HEIGHT; ++y)
    {
        for (auto x{ 0 }; x < Game::WORLD_WIDTH; ++x)
        {
            if (!IsIslandOnPosition(x, y, m_island5List))
            {
                auto waterGfx{ m_housesJsonFile->GetBuildings().at(waterId).gfx };
                waterGfx += (y + x * 3) % 12;

                const auto& waterBshTexture{ m_stadtfldFile->GetBshTexture(waterGfx) };

                chunk::TileGraphic deepWaterTileGraphic;
                deepWaterTileGraphic.tileGfxInfo.gfxIndex = waterGfx;
                deepWaterTileGraphic.mapPosition.x = x;
                deepWaterTileGraphic.mapPosition.y = y;

                auto screenPosition{ chunk::TileUtil::MapToScreen(x, y, m_zoom.GetXRaster(), m_zoom.GetYRaster()) };

                const auto adjustHeight{ chunk::TileUtil::AdjustHeight(
                    m_zoom.GetYRaster(),
                    static_cast<int>(chunk::TileHeight::SEA_LEVEL),
                    m_zoom.GetElevation())
                };

                screenPosition.y += adjustHeight;

                screenPosition.x -= waterBshTexture.width;
                screenPosition.y -= waterBshTexture.height;

                deepWaterTileGraphic.screenPosition = glm::vec2(screenPosition.x, screenPosition.y);
                deepWaterTileGraphic.size = glm::vec2(waterBshTexture.width, waterBshTexture.height);

                t_graphicTiles.push_back(deepWaterTileGraphic);
                m_deepWaterIndex[chunk::TileUtil::GetIndexFrom2D(x, y)] = static_cast<int>(t_graphicTiles.size()) - 1;
            }
        }
    }
}

int sg::file::GamFile::DeepWaterTileInVbo(const int t_mapX, const int t_mapY)
{
    if (t_mapX < 0 || t_mapY < 0)
    {
        return NO_DEEP_WATER;
    }

    return m_deepWaterIndex[chunk::TileUtil::GetIndexFrom2D(t_mapX, t_mapY)];
}

//-------------------------------------------------
// Islands area
//-------------------------------------------------

void sg::file::GamFile::InitIslandsArea()
{
    Log::SG_LOG_DEBUG("[GamFile::InitIslandsArea()] Create a model for each Island5.");

    for (auto& island5 : m_island5List)
    {
        m_islandModels.emplace_back(std::make_unique<renderer::IslandModel>(
            m_parentLayer->GetParentGame()->GetShaderManager(), m_zoom, island5.get(), m_stadtfldFile)
        );
    }
}
