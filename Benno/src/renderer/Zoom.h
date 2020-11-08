#pragma once

#include <unordered_map>
#include <vector>

namespace sg::renderer
{
    enum class Zoom
    {
        SGFX, MGFX, GFX
    };

    inline static std::vector<Zoom> const ZOOM_TYPES{ Zoom::SGFX, Zoom::MGFX, Zoom::GFX };

    inline static std::unordered_map<std::string, Zoom> const STRING_TO_ZOOM{
        { "SGFX", Zoom::SGFX },
        { "MGFX", Zoom::MGFX },
        { "GFX", Zoom::GFX }
    };

    inline static std::unordered_map<Zoom, std::string> const ZOOM_TO_STRING{
        { Zoom::SGFX, "SGFX" },
        { Zoom::MGFX, "MGFX" },
        { Zoom::GFX, "GFX" }
    };
}
