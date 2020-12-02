#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "Island.h"
#include "Tile.h"
#include "TileGraphic.h"

namespace sg::data
{
    class HousesJsonFile;
}

namespace sg::chunk
{
    class IslandHouse;

    struct Island5Data
    {
        uint8_t islandNumber;
        uint8_t width;
        uint8_t height;
        uint8_t strtduerrflg : 1;
        uint8_t nofixflg : 1;
        uint8_t vulkanflg : 1;
        uint16_t posx;
        uint16_t posy;
        uint16_t hirschreviercnt;
        uint16_t speedcnt;
        uint8_t stadtplayernr[11]; // todo
        uint8_t vulkancnt;
        uint8_t schatzflg;
        uint8_t rohstanz;
        uint8_t eisencnt;
        uint8_t playerflags;
        OreMountainData eisenberg[4]; // todo
        OreMountainData vulkanberg[4]; // todo
        Island::Fertility fertility;
        uint16_t fileNumber;
        Island::Size size;
        Island::Climate climate;
        uint8_t modifiedFlag;
        uint8_t duerrproz;
        uint8_t rotier;
        uint32_t seeplayerflags;
        uint32_t duerrcnt;
        uint32_t leer3;
    };

    class Island5
    {
    public:
        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        bool selected{ false };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Island5() = delete;

        Island5(const std::vector<uint8_t>& t_chunkData, std::shared_ptr<data::HousesJsonFile> t_housesJsonFile);

        Island5(const Island5& t_other) = delete;
        Island5(Island5&& t_other) noexcept = delete;
        Island5& operator=(const Island5& t_other) = delete;
        Island5& operator=(Island5&& t_other) noexcept = delete;

        ~Island5();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] const Island5Data& GetIsland5Data() const noexcept;
        [[nodiscard]] Tile GetTileFromLayer(int t_x, int t_y) const;
        [[nodiscard]] TileGfxInfo GetTileGfxInfo(Tile t_tile) const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void AddIslandHouse(std::unique_ptr<IslandHouse> t_islandHouse);

        //-------------------------------------------------
        // Island5 Layer
        //-------------------------------------------------

        void InitLayer();

    protected:

    private:
        std::shared_ptr<data::HousesJsonFile> m_housesJsonFile;

        Island5Data m_island5Data{};

        std::vector<std::shared_ptr<IslandHouse>> m_islandHouses;
        std::vector<std::shared_ptr<IslandHouse>> m_finalIslandHouses;

        std::shared_ptr<IslandHouse> m_topLayer;
        std::shared_ptr<IslandHouse> m_bottomLayer;

        //-------------------------------------------------
        // Read data
        //-------------------------------------------------

        void ReadIsland5Data(const std::vector<uint8_t>& t_chunkData);
    };
}
