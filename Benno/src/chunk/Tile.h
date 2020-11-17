#pragma once

#include <cstdint>

namespace sg::chunk
{
    struct Tile
    {
        /**
         * @brief The tile gaphic Id, see haeuser.cod for reference.
         */
        uint16_t graphicId;

        /**
         * @brief The x position on island.
         */
        uint8_t xPosOnIsland;

        /**
         * @brief The y position on island.
         */
        uint8_t yPosOnIsland;

        /**
         * @brief The orientation/rotation of the tile.
         */
        uint32_t orientation : 2;

        /**
         * @brief The animation step for the tile.
         */
        uint32_t animationCount : 4;

        /**
         * @brief The parent Island number.
         */
        uint32_t islandNumber : 8;

        /**
         * @brief The parent City number.
         */
        uint32_t cityNumber : 3;

        /**
         * @brief An unknown random number.
         */
        uint32_t randomNumber : 5;

        /**
         * @brief The player that has occupies this tile.
         */
        uint32_t playerNumber : 4;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Tile()
        {
            memset(this, 0, sizeof(Tile));
        }

        explicit Tile(const uint16_t t_graphicId)
            : Tile()
        {
            graphicId = t_graphicId;
        }
    };
}
