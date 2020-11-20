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
#include "renderer/DeepWaterRenderer.h"
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

    // todo: temp code, just for testing, slow...very slow
    for (const auto& island5 : m_island5List)
    {
        const auto yPos{ island5->GetIsland5Data().posy };
        const auto xPos{ island5->GetIsland5Data().posx };
        const auto height{ island5->GetIsland5Data().height };
        const auto width{ island5->GetIsland5Data().width };

        for (auto y{ yPos }; y < yPos + height; ++y)
        {
            for (auto x{ xPos }; x < xPos + width; ++x)
            {
                const auto tile{ island5->GetTileFromLayer(x - xPos, y - yPos) };
                const auto field{ island5->GetGraphicForTile(tile) };
                const auto& bshTexture{ m_bshFile->GetBshTexture(field.gfxIndex) };

                const auto sx{ (x - y + GameLayer::WORLD_HEIGHT) * m_zoom.GetXRaster() };
                const auto sy{ (x + y) * m_zoom.GetYRaster() + 2 * m_zoom.GetYRaster() - field.groundHeight / m_zoom.GetElevation() };

                const auto position{ glm::vec2(sx - static_cast<float>(bshTexture.width) / 2.0f, sy - bshTexture.height) };

                m_meshRenderer.Render(
                    position.x,
                    position.y,
                    bshTexture,
                    t_camera
                );
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
        deepWaterTextureBuffer.push_back(tile.gfxIndex - renderer::DeepWaterRenderer::START_GFX_INDEX);
    }

    m_deepWaterRenderer = std::make_unique<renderer::DeepWaterRenderer>(m_bshFile, std::move(deepWaterModelMatrices), std::move(deepWaterTextureBuffer));
    m_deepWaterRenderer->Init(m_zoom);
}

void sg::file::GamFile::CreateDeepWaterGraphicTiles(std::vector<chunk::TileGraphic>& t_graphicTiles) const
{
    for (auto y{ 0 }; y < GameLayer::WORLD_HEIGHT; ++y)
    {
        for (auto x{ 0 }; x < GameLayer::WORLD_WIDTH; ++x)
        {
            if (!IsIslandOnPosition(x, y, m_island5List))
            {
                chunk::TileGraphic deepWaterTileGraphic{ m_housesJsonFile->GetBuildings().at(1201).gfx, 0 };
                deepWaterTileGraphic.gfxIndex += (y + x * 3) % 12;

                AddTileGraphicToList(x, y, t_graphicTiles, deepWaterTileGraphic);
            }
        }
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::file::GamFile::AddTileGraphicToList(const int t_x, const int t_y, std::vector<chunk::TileGraphic>& t_graphicTiles, chunk::TileGraphic& t_tileGraphic) const
{
    const auto& bshTexture{ m_bshFile->GetBshTexture(t_tileGraphic.gfxIndex) };

    const auto sx{ (t_x - t_y + GameLayer::WORLD_HEIGHT) * m_zoom.GetXRaster() };
    const auto sy{ (t_x + t_y) * m_zoom.GetYRaster() + 2 * m_zoom.GetYRaster() - t_tileGraphic.groundHeight / m_zoom.GetElevation() };

    const auto position{ glm::vec2(sx - static_cast<float>(bshTexture.width) / 2.0f, sy - bshTexture.height) };
    const auto size{ glm::vec2(bshTexture.width, bshTexture.height) };

    t_tileGraphic.screenPosition = position;
    t_tileGraphic.size = size;

    t_graphicTiles.push_back(t_tileGraphic);
}

bool sg::file::GamFile::IsIslandOnPosition(const int t_x, const int t_y, const std::vector<std::unique_ptr<chunk::Island5>>& t_island5List)
{
    for (const auto& island5 : t_island5List)
    {
        if (t_x >= island5->GetIsland5Data().posx &&
            t_y >= island5->GetIsland5Data().posy &&
            t_x < island5->GetIsland5Data().posx + island5->GetIsland5Data().width &&
            t_y < island5->GetIsland5Data().posy + island5->GetIsland5Data().height
            )
        {
            return true;
        }
    }

    return false;
}
