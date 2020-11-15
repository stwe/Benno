#include "IslandHouse.h"
#include "SgAssert.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::chunk::IslandHouse::IslandHouse(const std::vector<uint8_t>& t_chunkData, Island5* t_parentIsland5)
    : m_parentIsland5{ t_parentIsland5 }
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
}

sg::chunk::IslandHouse::~IslandHouse()
{
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
}
