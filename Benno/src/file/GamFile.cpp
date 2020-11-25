#include <unordered_set>
#include <glm/vec2.hpp>
#include "GamFile.h"
#include "BshFile.h"
#include "BshTexture.h"
#include "GameLayer.h"
#include "Log.h"
#include "data/HousesJsonFile.h"
#include "chunk/Island5.h"
#include "chunk/IslandHouse.h"
#include "chunk/Chunk.h"
#include "chunk/TileUtil.h"
#include "renderer/DeepWaterRenderer.h"
#include "renderer/IslandsRenderer.h"
#include "camera/OrthographicCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::file::GamFile::GamFile(
    const std::string& t_filePath,
    std::shared_ptr<BshFile> t_bshFile,
    std::shared_ptr<data::HousesJsonFile> t_housesJsonFile,
    const renderer::Zoom& t_zoom
)
    : BinaryFile(t_filePath)
    , m_bshFile{ std::move(t_bshFile) }
    , m_housesJsonFile{ std::move(t_housesJsonFile) }
    , m_zoom{ t_zoom }
{
    Log::SG_LOG_DEBUG("[GamFile::GamFile()] Creates GamFile object from file {}.", t_filePath);
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

void sg::file::GamFile::Render(const camera::OrthographicCamera& t_camera)
{
    m_deepWaterRenderer->Render(t_camera);
    m_islandsRenderer->Render(t_camera);
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

    CreateDeepWaterGraphicTiles(deepWaterGraphicTiles);

    for (const auto& tile : deepWaterGraphicTiles)
    {
        deepWaterModelMatrices.push_back(tile.GetModelMatrix());
        deepWaterTextureBuffer.push_back(tile.tileGfxInfo.gfxIndex - renderer::DeepWaterRenderer::START_GFX_INDEX);
    }

    m_deepWaterRenderer = std::make_unique<renderer::DeepWaterRenderer>(m_bshFile, std::move(deepWaterModelMatrices), std::move(deepWaterTextureBuffer));
    m_deepWaterRenderer->Init(m_zoom);
}

void sg::file::GamFile::CreateDeepWaterGraphicTiles(std::vector<chunk::TileGraphic>& t_graphicTiles) const
{
    constexpr auto waterId{ 1201 };

    for (auto y{ 0 }; y < GameLayer::WORLD_HEIGHT; ++y)
    {
        for (auto x{ 0 }; x < GameLayer::WORLD_WIDTH; ++x)
        {
            if (!IsIslandOnPosition(x, y, m_island5List))
            {
                auto waterGfx{ m_housesJsonFile->GetBuildings().at(waterId).gfx };
                waterGfx += (y + x * 3) % 12;

                const auto& waterBshTexture{ m_bshFile->GetBshTexture(waterGfx) };

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
            }
        }
    }
}

//-------------------------------------------------
// Islands area
//-------------------------------------------------

void sg::file::GamFile::InitIslandsArea()
{
    Log::SG_LOG_DEBUG("[GamFile::InitIslandsArea()] Create data for the Islands Renderer.");

    std::vector<chunk::TileGraphic> islandsGraphicTiles;
    std::vector<glm::mat4> islandsModelMatrices;
    std::vector<int> islandsTextureBuffer;
    std::vector<float> yBuffer;
    std::unordered_map<int, int> gfxIndexMap;

    CreateIslandsGraphicTiles(islandsGraphicTiles);

    for (const auto& tile : islandsGraphicTiles)
    {
        islandsModelMatrices.push_back(tile.GetModelMatrix());
    }

    CreateIslandsTextureIndex(islandsGraphicTiles, islandsTextureBuffer, yBuffer, gfxIndexMap);

    m_islandsRenderer = std::make_unique<renderer::IslandsRenderer>(
        m_bshFile,
        std::move(islandsModelMatrices),
        std::move(islandsTextureBuffer),
        std::move(yBuffer),
        std::move(gfxIndexMap)
        );

    m_islandsRenderer->Init(m_zoom);
}

void sg::file::GamFile::CreateIslandsGraphicTiles(std::vector<chunk::TileGraphic>& t_graphicTiles) const
{
    for (auto y{ 0 }; y < GameLayer::WORLD_HEIGHT; ++y)
    {
        for (auto x{ 0 }; x < GameLayer::WORLD_WIDTH; ++x)
        {
            auto* island{ IsIslandOnPosition(x, y, m_island5List) };
            if (island)
            {
                const auto tile{ island->GetTileFromLayer(x - island->GetIsland5Data().posx, y - island->GetIsland5Data().posy) };
                const auto tileGfxInfo{ island->GetTileGfxInfo(tile) };
                const auto& bshTexture{ m_bshFile->GetBshTexture(tileGfxInfo.gfxIndex) };

                chunk::TileGraphic tileGraphic;
                tileGraphic.tileGfxInfo = tileGfxInfo;
                tileGraphic.mapPosition.x = x;
                tileGraphic.mapPosition.y = y;

                auto screenPosition{ chunk::TileUtil::MapToScreen(x, y, m_zoom.GetXRaster(), m_zoom.GetYRaster()) };

                const auto adjustHeight{ chunk::TileUtil::AdjustHeight(
                    m_zoom.GetYRaster(),
                    static_cast<int>(tileGraphic.tileGfxInfo.tileHeight),
                    m_zoom.GetElevation())
                };

                screenPosition.y += adjustHeight;

                screenPosition.x -= bshTexture.width;
                screenPosition.y -= bshTexture.height;

                tileGraphic.screenPosition = glm::vec2(screenPosition.x, screenPosition.y);
                tileGraphic.size = glm::vec2(bshTexture.width, bshTexture.height);

                t_graphicTiles.push_back(tileGraphic);
            }
        }
    }
}

void sg::file::GamFile::CreateIslandsTextureIndex(
    const std::vector<chunk::TileGraphic>& t_graphicTiles,
    std::vector<int>& t_islandsTextureBuffer,
    std::vector<float>& t_yBuffer,
    std::unordered_map<int, int>& t_gfxIndexMap
)
{
    t_islandsTextureBuffer.resize(t_graphicTiles.size());
    t_yBuffer.resize(t_graphicTiles.size());

    std::unordered_set<int> islandTexturesToLoad;
    for (const auto& tile : t_graphicTiles)
    {
        islandTexturesToLoad.emplace(tile.tileGfxInfo.gfxIndex);
    }

    auto zOffset{ 0 };
    for (auto toLoad : islandTexturesToLoad)
    {
        t_gfxIndexMap.emplace(toLoad, zOffset);
        zOffset++;
    }

    auto instance{ 0 };
    for (const auto& tile : t_graphicTiles)
    {
        t_islandsTextureBuffer[instance] = t_gfxIndexMap.at(tile.tileGfxInfo.gfxIndex);
        t_yBuffer[instance] = tile.size.y;

        instance++;
    }
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
