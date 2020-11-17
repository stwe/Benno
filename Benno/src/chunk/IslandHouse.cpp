#include "IslandHouse.h"
#include "Island5.h"
#include "SgAssert.h"
#include "SgException.h"
#include "data/HousesJsonFile.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::chunk::IslandHouse::IslandHouse(const std::vector<uint8_t>& t_chunkData, Island5* t_parentIsland5, std::shared_ptr<data::HousesJsonFile> t_housesJsonFile)
    : m_parentIsland5{ t_parentIsland5 }
    , m_housesJsonFile{ std::move(t_housesJsonFile) }
{
    SG_ASSERT(t_parentIsland5, "[IslandHouse::IslandHouse()] Null pointer.");

    Log::SG_LOG_DEBUG("[IslandHouse::IslandHouse()] Creates IslandHouse object.");

    if (t_chunkData.empty())
    {
        m_nrOfRawElements = 0;
        Log::SG_LOG_DEBUG("[IslandHouse::IslandHouse()] Skip reading. There were no tiles found.");
    }
    else
    {
        m_nrOfRawElements = t_chunkData.size() / sizeof(Tile);
        Log::SG_LOG_DEBUG("[IslandHouse::IslandHouse()] Detected {} tiles.", m_nrOfRawElements);
        ReadIslandHouseData(t_chunkData);
    }

    CreateLayerTiles();
}

sg::chunk::IslandHouse::~IslandHouse()
{
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

sg::chunk::Tile sg::chunk::IslandHouse::GetTile(const int t_x, const int t_y) const
{
    return m_tiles.at(static_cast<size_t>(t_y) * m_parentIsland5->GetIsland5Data().width + t_x);
}

//-------------------------------------------------
// Read data
//-------------------------------------------------

void sg::chunk::IslandHouse::ReadIslandHouseData(const std::vector<uint8_t>& t_chunkData)
{
    Log::SG_LOG_DEBUG("[IslandHouse::ReadIslandHouseData()] Start reading the IslandHouse tile data...");

    m_rawTiles.resize(m_nrOfRawElements);
    memcpy(m_rawTiles.data(), t_chunkData.data(), t_chunkData.size());

    Log::SG_LOG_DEBUG("[IslandHouse::ReadIslandHouseData()] IslandHouse tile data read successfully.");
}

//-------------------------------------------------
// Tiles
//-------------------------------------------------

void sg::chunk::IslandHouse::CreateLayerTiles()
{
    const auto width{ m_parentIsland5->GetIsland5Data().width };
    const auto height{ m_parentIsland5->GetIsland5Data().height };

    m_tiles.resize(static_cast<size_t>(width) * height, Tile(NO_GRAPHIC));

    for (auto i{ 0u }; i < m_nrOfRawElements; ++i)
    {
        const auto& rawTile{ m_rawTiles[i] };

        if (IsValidTilePosition(rawTile.xPosOnIsland, rawTile.yPosOnIsland))
        {
            const auto& building{ m_housesJsonFile->GetBuildings().at(rawTile.graphicId) };

            auto w{ 0 };
            auto h{ 0 };
            if (rawTile.orientation % 2 == 0)
            {
                w = building.width;
                h = building.height;
            }
            else
            {
                w = building.height;
                h = building.width;
            }

            for (auto y{ 0 }; y < h && IsValidTilePosY(rawTile.yPosOnIsland + y); ++y)
            {
                for (auto x{ 0 }; x < w && IsValidTilePosX(rawTile.xPosOnIsland + x); ++x)
                {
                    const auto targetIndex{ (rawTile.yPosOnIsland + y) * width + rawTile.xPosOnIsland + x };

                    m_tiles[targetIndex] = rawTile;

                    m_tiles[targetIndex].graphicId = rawTile.graphicId;
                    m_tiles[targetIndex].xPosOnIsland = x;
                    m_tiles[targetIndex].yPosOnIsland = y;
                }
            }
        }
        else
        {
            throw SG_EXCEPTION("[IslandHouse::CreateLayerTiles()] Invalid Tile position.");
        }
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool sg::chunk::IslandHouse::IsValidIslandNumber(const int t_islandNumber) const noexcept
{
    return t_islandNumber == m_parentIsland5->GetIsland5Data().islandNumber;
}

bool sg::chunk::IslandHouse::IsValidTilePosition(const int t_xPosOnIsland, const int t_yPosOnIsland) const noexcept
{
    return IsValidTilePosX(t_xPosOnIsland) && IsValidTilePosY(t_yPosOnIsland);
}

bool sg::chunk::IslandHouse::IsValidTilePosX(const int t_xPosOnIsland) const noexcept
{
    return t_xPosOnIsland < m_parentIsland5->GetIsland5Data().width;
}

bool sg::chunk::IslandHouse::IsValidTilePosY(const int t_yPosOnIsland) const noexcept
{
    return t_yPosOnIsland < m_parentIsland5->GetIsland5Data().height;
}
