#include "Island5.h"
#include "Log.h"
#include "SgAssert.h"
#include "IslandHouse.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::chunk::Island5::Island5(const std::vector<uint8_t>& t_chunkData)
{
    SG_ASSERT(!t_chunkData.empty(), "[Island5::Island5()] Invalid chunk data.");

    ReadIsland5Data(t_chunkData);
}

sg::chunk::Island5::~Island5()
{
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

const sg::chunk::Island5Data& sg::chunk::Island5::GetIsland5Data() const noexcept
{
    return m_island5Data;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::chunk::Island5::AddIslandHouse(std::unique_ptr<IslandHouse> t_islandHouse)
{
    m_islandHouses.emplace_back(std::move(t_islandHouse));
}

//-------------------------------------------------
// Read data
//-------------------------------------------------

void sg::chunk::Island5::ReadIsland5Data(const std::vector<uint8_t>& t_chunkData)
{
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] Start reading the Island5 data...");

    SG_ASSERT(sizeof(m_island5Data) == t_chunkData.size(), "[Island5::ReadIsland5Data()] Invalid chunk data.");
    memcpy(reinterpret_cast<char*>(&m_island5Data), t_chunkData.data(), t_chunkData.size());

    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] Island5 data read successfully.");
}
