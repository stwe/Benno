#include "Island5.h"
#include "Log.h"

sg::chunk::Island5::Island5(const std::vector<uint8_t>& t_chunkData)
{
    if (!t_chunkData.empty())
    {
        ReadIsland5Data(t_chunkData);
    }
}

sg::chunk::Island5::~Island5()
{
}

void sg::chunk::Island5::ReadIsland5Data(const std::vector<uint8_t>& t_chunkData)
{
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] Start reading the Island5 data...");

    memcpy(reinterpret_cast<char*>(&m_island5Data), t_chunkData.data(), t_chunkData.size());

    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] Island5 data read successfully.");
}
