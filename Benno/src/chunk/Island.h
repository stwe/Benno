#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

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

    struct Island
    {
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
            WINE_AND_COCOA = 0x11E1,
            UNKNOWN0 = 4549,
            UNKNOWN1 = 4515,
            UNKNOWN2 = 4501,
        };

        inline static std::unordered_map<Fertility, std::string> const FERTILITY_STRINGS
        {
            { Fertility::RANDOM, "Random" },
            { Fertility::NONE, "None" },
            { Fertility::TOBACCO_ONLY, "Tobacco only" },
            { Fertility::WINE_ONLY, "Wine only" },
            { Fertility::SUGAR_ONLY, "Sugar only" },
            { Fertility::COCOA_ONLY, "Cocoa only" },
            { Fertility::WOOL_ONLY, "Wool only" },
            { Fertility::SPICES_ONLY, "Spices only" },
            { Fertility::TOBACCO_AND_SPICES, "Tobacco and spices" },
            { Fertility::TOBACCO_AND_SUGAR, "Tobacco and sugar" },
            { Fertility::SPICES_AND_SUGAR, "Spices and sugar" },
            { Fertility::WOOL_AND_WINE, "Wool and wine" },
            { Fertility::WOOL_AND_COCOA, "Wool and cocoa" },
            { Fertility::WINE_AND_COCOA, "Wine and cocoa" },
            { Fertility::UNKNOWN0, "Unknown0" },
            { Fertility::UNKNOWN1, "Unknown1" },
            { Fertility::UNKNOWN2, "Unknown2" },
        };

        static const std::string& FertilityToString(const Fertility t_fertilityId)
        {
            return FERTILITY_STRINGS.find(t_fertilityId)->second;
        }

        enum class Size : uint16_t
        {
            LITTLE = 0,
            MIDDLE = 1,
            MEDIAN = 2,
            BIG = 3,
            LARGE = 4
        };

        inline static std::unordered_map<Size, std::string> const SIZE_STRINGS
        {
            { Size::LITTLE, "Little" },
            { Size::MIDDLE, "Middle" },
            { Size::MEDIAN, "Median" },
            { Size::BIG, "Big" },
            { Size::LARGE, "Large" }
        };

        static const std::string& SizeToString(const Size t_sizeId)
        {
            return SIZE_STRINGS.find(t_sizeId)->second;
        }

        enum class Climate : uint8_t
        {
            NORTH = 0,
            SOUTH = 1,
            ANY = 2
        };

        inline static std::unordered_map<Climate, std::string> const CLIMATE_STRINGS
        {
            { Climate::NORTH, "North" },
            { Climate::SOUTH, "South" },
            { Climate::ANY, "Any" }
        };

        static const std::string& ClimateToString(const Climate t_climateId)
        {
            return CLIMATE_STRINGS.find(t_climateId)->second;
        }
    };
}
