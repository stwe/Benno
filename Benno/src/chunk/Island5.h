#pragma once

#include <cstdint>
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
        IslandModified modifiedFlag; // flag that indicates if the island is `original` (empty INSELHAUS chunk) or `modified` (filled INSELHAUS chunk).
        uint8_t duerrproz;
        uint8_t rotier;
        uint32_t seeplayerflags;
        uint32_t duerrcnt;
        uint32_t leer3;
    };

    class Island5
    {
    public:

    protected:

    private:

    };
}
