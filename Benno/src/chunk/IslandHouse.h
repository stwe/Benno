#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "Tile.h"

namespace sg::data
{
    class HousesJsonFile;
}

namespace sg::chunk
{
    class Island5;

    class IslandHouse
    {
    public:
        static constexpr auto NO_GRAPHIC{ 0xFFFF };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        IslandHouse(const std::vector<uint8_t>& t_chunkData, Island5* t_parentIsland5, std::shared_ptr<data::HousesJsonFile> t_housesJsonFile);

        IslandHouse(const IslandHouse& t_other) = delete;
        IslandHouse(IslandHouse&& t_other) noexcept = delete;
        IslandHouse& operator=(const IslandHouse& t_other) = delete;
        IslandHouse& operator=(IslandHouse&& t_other) noexcept = delete;

        ~IslandHouse();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] Tile GetTile(int t_x, int t_y) const;

    protected:

    private:
        Island5* m_parentIsland5{ nullptr };
        std::shared_ptr<data::HousesJsonFile> m_housesJsonFile;

        size_t m_nrOfRawElements{ 0 };

        std::vector<Tile> m_rawTiles;
        std::vector<Tile> m_tiles;

        //-------------------------------------------------
        // Read data
        //-------------------------------------------------

        void ReadIslandHouseData(const std::vector<uint8_t>& t_chunkData);

        //-------------------------------------------------
        // Tiles
        //-------------------------------------------------

        void CreateLayerTiles();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        [[nodiscard]] bool IsValidIslandNumber(int t_islandNumber) const noexcept;
        [[nodiscard]] bool IsValidTilePosition(int t_xPosOnIsland, int t_yPosOnIsland) const noexcept;
        [[nodiscard]] bool IsValidTilePosX(int t_xPosOnIsland) const noexcept;
        [[nodiscard]] bool IsValidTilePosY(int t_yPosOnIsland) const noexcept;
    };
}
