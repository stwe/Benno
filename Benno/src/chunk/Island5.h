#pragma once

#include <cstdint>
#include <vector>
#include "Island.h"

namespace sg::chunk
{
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
        uint8_t stadtplayernr[11];
        uint8_t vulkancnt;
        uint8_t schatzflg;
        uint8_t rohstanz;
        uint8_t eisencnt;
        uint8_t playerflags;
        OreMountainData eisenberg[4];
        OreMountainData vulkanberg[4];
        Fertility fertility;
        uint16_t fileNumber;
        IslandSize size;
        IslandClimate climate;
        uint8_t modifiedFlag; // flag that indicates if the island is `original` (empty INSELHAUS chunk) or `modified` (filled INSELHAUS chunk).
        uint8_t duerrproz;
        uint8_t rotier;
        uint32_t seeplayerflags;
        uint32_t duerrcnt;
        uint32_t leer3;
    };

    class Island5
    {
    public:

        Island5() = delete;

        explicit Island5(const std::vector<uint8_t>& t_chunkData);

        Island5(const Island5& t_other) = delete;
        Island5(Island5&& t_other) noexcept = delete;
        Island5& operator=(const Island5& t_other) = delete;
        Island5& operator=(Island5&& t_other) noexcept = delete;

        ~Island5();

        void ReadIsland5Data(const std::vector<uint8_t>& t_chunkData);

    protected:

    private:
        Island5Data m_island5Data;
    };
}
