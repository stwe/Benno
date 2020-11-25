#pragma once

#include <glm/vec2.hpp>

namespace sg::chunk
{
    struct TileUtil
    {
        static glm::ivec2 ScreenToMap(const float t_screenX, const float t_screenY, const int t_tileWidthHalf, const int t_tileHeightHalf)
        {
            const auto tileWidthHalf{ static_cast<float>(t_tileWidthHalf) };
            const auto tileHeightHalf{ static_cast<float>(t_tileHeightHalf) };

            return glm::ivec2(
                (t_screenX / tileWidthHalf + t_screenY / tileHeightHalf) / 2,
                (t_screenY / tileHeightHalf - t_screenX / tileHeightHalf) / 2
            );
        }

        static glm::ivec2 MapToScreen(const int t_mapX, const int t_mapY, const int t_tileWidthHalf, const int t_tileHeightHalf)
        {
            return glm::ivec2(
                (t_mapX - t_mapY) * t_tileWidthHalf,
                (t_mapX + t_mapY) * t_tileHeightHalf
            );
        }

        static int AdjustHeight(const int t_tileHeightHalf, const int t_tileHeight, const int t_elevation)
        {
            return 2 * t_tileHeightHalf - t_tileHeight / t_elevation;
        }
    };
}
