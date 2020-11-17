#include "GamFile.h"
#include "BshFile.h"
#include "BshTexture.h"
#include "Log.h"
#include "data/HousesJsonFile.h"
#include "chunk/Island5.h"
#include "chunk/IslandHouse.h"
#include "chunk/Chunk.h"
#include "renderer/MeshRenderer.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::file::GamFile::GamFile(const std::string& t_filePath, std::shared_ptr<data::HousesJsonFile> t_housesJsonFile)
    : BinaryFile(t_filePath)
    , m_housesJsonFile{ std::move(t_housesJsonFile) }
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

void sg::file::GamFile::Render(BshFile* t_bshFile, renderer::MeshRenderer* t_renderer, const glm::mat4& t_mat) const
{
    const auto xRaster{ 8 };
    const auto yRaster{ 4 };
    const auto elevation{ 4 };
    const auto worldHeight{ 350 };

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
                const auto& bshTexture{ t_bshFile->GetBshTexture(field.gfxIndex) };

                const auto sx{ (x - y + worldHeight) * xRaster };
                const auto sy{ (x + y) * yRaster + 2 * yRaster - field.groundHeight / elevation };

                const auto position{ glm::vec2(sx - static_cast<float>(bshTexture.width) / 2.0f, sy - bshTexture.height) };

                t_renderer->Render(
                    position.x - 3000.0f,
                    position.y - 2000.0f,
                    bshTexture,
                    t_mat
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

    Log::SG_LOG_DEBUG("[GamFile::ReadContentFromChunkData()] Savegame data read successfully.");
}

//-------------------------------------------------
// Island5 Layer
//-------------------------------------------------

void sg::file::GamFile::InitIsland5Layer()
{
    for (auto& island5 : m_island5List)
    {
        island5->InitLayer();
    }
}
