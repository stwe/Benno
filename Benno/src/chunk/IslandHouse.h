#pragma once

#include <cstdint>
#include <vector>
#include "Tile.h"

namespace sg::chunk
{
    class Island5;

    class IslandHouse
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        IslandHouse(const std::vector<uint8_t>& t_chunkData, Island5* t_parentIsland5);

        IslandHouse(const IslandHouse& t_other) = delete;
        IslandHouse(IslandHouse&& t_other) noexcept = delete;
        IslandHouse& operator=(const IslandHouse& t_other) = delete;
        IslandHouse& operator=(IslandHouse&& t_other) noexcept = delete;

        ~IslandHouse();

    protected:

    private:
        Island5* m_parentIsland5{ nullptr };

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
    };
}
