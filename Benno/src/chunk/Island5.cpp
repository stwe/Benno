#include "Island5.h"
#include "Log.h"
#include "SgAssert.h"
#include "IslandHouse.h"
#include "SgException.h"
#include "data/HousesJsonFile.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::chunk::Island5::Island5(const std::vector<uint8_t>& t_chunkData, std::shared_ptr<data::HousesJsonFile> t_housesJsonFile)
    : m_housesJsonFile{ std::move(t_housesJsonFile) }
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

sg::chunk::Tile sg::chunk::Island5::GetTileFromLayer(const int t_x, const int t_y) const
{
    auto tile{ m_topLayer->GetTile(t_x, t_y) };
    if (tile.graphicId == IslandHouse::NO_GRAPHIC)
    {
        tile = m_bottomLayer->GetTile(t_x, t_y);
    }

    const auto xPos{ tile.xPosOnIsland };
    const auto yPos{ tile.yPosOnIsland };

    if (yPos > t_y || xPos > t_x)
    {
        return tile;
    }

    tile = m_topLayer->GetTile(t_x - xPos, t_y - yPos);
    if (tile.graphicId == IslandHouse::NO_GRAPHIC)
    {
        tile = m_bottomLayer->GetTile(t_x - xPos, t_y - yPos);
    }

    tile.xPosOnIsland = xPos;
    tile.yPosOnIsland = yPos;

    return tile;
}

sg::chunk::TileGraphic sg::chunk::Island5::GetGraphicForTile(const Tile t_tile) const
{
    TileGraphic targetField;

    if (t_tile.graphicId == IslandHouse::NO_GRAPHIC)
    {
        targetField.gfxIndex = -1;
        targetField.groundHeight = 0;

        return targetField;
    }

    const auto& building{ m_housesJsonFile->GetBuildings().at(t_tile.graphicId) };

    auto index{ building.gfx };

    auto directions{ 1 };
    if (building.rotate > 0)
    {
        directions = 4;
    }

    auto aniSteps{ 1 };
    if (building.animAnz > 0)
    {
        aniSteps = building.animAnz;
    }

    index += building.rotate * (t_tile.orientation % directions);

    switch (t_tile.orientation)
    {
        case 0: index += t_tile.yPosOnIsland * building.width + t_tile.xPosOnIsland;
            break;
        case 1: index += (building.height - t_tile.xPosOnIsland - 1) * building.width + t_tile.yPosOnIsland;
            break;
        case 2: index += (building.height - t_tile.yPosOnIsland - 1) * building.width + (building.width - t_tile.xPosOnIsland - 1);
            break;
        case 3: index += t_tile.xPosOnIsland * building.width + (building.width - t_tile.yPosOnIsland - 1);
            break;
        default: Log::SG_LOG_WARN("Unknow rotation.");
    }

    index += building.width * building.height * directions * (t_tile.animationCount % aniSteps);

    targetField.gfxIndex = index;
    targetField.groundHeight = 0;

    if (building.posoffs == 20)
    {
        targetField.groundHeight = building.posoffs;
    }

    return targetField;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::chunk::Island5::AddIslandHouse(std::unique_ptr<IslandHouse> t_islandHouse)
{
    // continue working with a shared_ptr...
    m_islandHouses.emplace_back(std::move(t_islandHouse));
}

//-------------------------------------------------
// Island5 Layer
//-------------------------------------------------

void sg::chunk::Island5::InitLayer()
{
    if (m_islandHouses.empty())
    {
        throw SG_EXCEPTION("[Island5::InitLayer()] No IslandHouse data found.");
    }

    // todo: hardcoded for the current savegame

    m_finalIslandHouses.push_back(m_islandHouses[0]);

    if (m_islandHouses.size() == 2)
    {
        m_finalIslandHouses.push_back(m_islandHouses[1]);
    }

    m_bottomLayer = m_finalIslandHouses[0];
    m_topLayer = m_finalIslandHouses[1];
}

//-------------------------------------------------
// Read data
//-------------------------------------------------

void sg::chunk::Island5::ReadIsland5Data(const std::vector<uint8_t>& t_chunkData)
{
    Log::SG_LOG_DEBUG("--------------------------------------------------------------");
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] Start reading the Island5 data...");

    SG_ASSERT(sizeof(m_island5Data) == t_chunkData.size(), "[Island5::ReadIsland5Data()] Invalid chunk data.");
    memcpy(reinterpret_cast<char*>(&m_island5Data), t_chunkData.data(), t_chunkData.size());

    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] Island5 data read successfully.");
    Log::SG_LOG_DEBUG("--------------------------------------------------------------");
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] width: {}", m_island5Data.width);
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] height: {}", m_island5Data.height);
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] xPos: {}", m_island5Data.posx);
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] yPos: {}", m_island5Data.posy);
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] modified: {}", m_island5Data.modifiedFlag == 1 ? "yes" : "no");
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] fertility: {}", Island::FertilityToString(m_island5Data.fertility));
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] size: {}", Island::SizeToString(m_island5Data.size));
    Log::SG_LOG_DEBUG("[Island5::ReadIsland5Data()] climate: {}", Island::ClimateToString(m_island5Data.climate));
}
