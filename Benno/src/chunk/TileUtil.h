#pragma once

#include <glm/vec2.hpp>
#include "Game.h"

namespace sg::chunk
{
    struct TileUtil
    {
        static glm::vec2 ScreenToMap(float t_screenX, const float t_screenY, const int t_tileWidth, const int t_tileHeight)
        {
            const auto tileWidth{ static_cast<float>(t_tileWidth) };
            const auto tileHeight{ static_cast<float>(t_tileHeight) };

            t_screenX += tileWidth * 0.5f;

            return {
                (t_screenX / tileWidth + t_screenY / tileHeight),
                (t_screenY / tileHeight - t_screenX / tileWidth)
            };
        }

        static glm::vec2 IslandScreenToMap(float t_screenX, float t_screenY, const int t_tileWidth, const int t_tileHeight)
        {
            const auto tileWidth{ static_cast<float>(t_tileWidth) };
            const auto tileHeight{ static_cast<float>(t_tileHeight) };

            t_screenX += tileWidth * 0.5f;
            t_screenY += tileHeight * 0.5f;

            return {
                (t_screenX / tileWidth + t_screenY / tileHeight),
                (t_screenY / tileHeight - t_screenX / tileWidth)
            };
        }

        static glm::ivec2 MapToScreen(const int t_mapX, const int t_mapY, const int t_tileWidthHalf, const int t_tileHeightHalf)
        {
            return {
                (t_mapX - t_mapY) * t_tileWidthHalf,
                (t_mapX + t_mapY) * t_tileHeightHalf
            };
        }

        static int AdjustHeight(const int t_tileHeightHalf, const int t_tileHeight, const int t_elevation)
        {
            return 2 * t_tileHeightHalf - t_tileHeight / t_elevation;
        }

        static int GetIndexFrom2D(const int t_mapX, const int t_mapY)
        {
            return t_mapY * Game::WORLD_WIDTH + t_mapX;
        }
    };
}
