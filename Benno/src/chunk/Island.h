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
        RANDOM = 0x0000,
        NONE = 0x1181,
        TOBACCO_ONLY = 0x1183,
        WINE_ONLY = 0x11A1,
        SUGAR_ONLY = 0x1189,
        COCOA_ONLY = 0x11C1,
        WOOL_ONLY = 0x1191,
        SPICES_ONLY = 0x1185,
        TOBACCO_AND_SPICES = 0x1187,
        TOBACCO_AND_SUGAR = 0x118B,
        SPICES_AND_SUGAR = 0x118D,
        WOOL_AND_WINE = 0x11B1,
        WOOL_AND_COCOA = 0x11D1,
        WINE_AND_COCOA = 0x11E1
    };

    enum class IslandSize : uint16_t
    {
        LITTLE = 0,
        MIDDLE = 1,
        MEDIAN = 2,
        BIG = 3,
        LARGE = 4
    };

    enum class IslandClimate : uint8_t
    {
        NORTH = 0,
        SOUTH = 1,
        ANY = 2
    };
}
