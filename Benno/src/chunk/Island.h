#pragma once

#include <cstdint>

namespace sg::chunk
{
    struct OreMountainData // Erzberg
    {
        uint8_t ware; // which good lays here?
        uint8_t posx; // position on island x
        uint8_t posy; // position on island y
        uint8_t playerflags; // which player knows this secret? (DANGER: PLAYER_MAX)
        uint8_t kind; // which kind?
        uint8_t empty; // empty
        uint16_t stock; // how much of this good lays here?
    };

    enum class Fertility : uint32_t
    {
        Random = 0x0000,
        None = 0x1181,
        TobaccoOnly = 0x1183,
        WineOnly = 0x11A1,
        SugarOnly = 0x1189,
        CocoaOnly = 0x11C1,
        WoolOnly = 0x1191,
        SpicesOnly = 0x1185,
        TobaccoAndSpices = 0x1187,
        TobaccoAndSugar = 0x118B,
        SpicesAndSugar = 0x118D,
        WoolAndWine = 0x11B1,
        WoolAndCocoa = 0x11D1,
        WineAndCocoa = 0x11E1
    };

    enum class IslandModified : uint8_t
    {
        False = 0,
        True = 1
    };

    enum class IslandSize : uint16_t
    {
        Little = 0,
        Middle = 1,
        Median = 2,
        Big = 3,
        Large = 4
    };

    enum class IslandClimate : uint8_t
    {
        North = 0,
        South = 1,
        Any = 2
    };
}
