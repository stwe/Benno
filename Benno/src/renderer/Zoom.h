#pragma once

#include <unordered_map>
#include <vector>

namespace sg::renderer
{
    struct Zoom
    {
        enum class ZoomId
        {
            SGFX, MGFX, GFX, NOT_ZOOMABLE
        };

        inline static std::vector<ZoomId> const ZOOM_TABLE{ ZoomId::SGFX, ZoomId::MGFX, ZoomId::GFX };

        inline static std::unordered_map<ZoomId, std::string> const ZOOM_STRINGS{
            { ZoomId::SGFX, "SGFX" },
            { ZoomId::MGFX, "MGFX" },
            { ZoomId::GFX, "GFX" }
        };

        static const std::string& ZoomToString(const ZoomId t_zoomId)
        {
            return ZOOM_STRINGS.find(t_zoomId)->second;
        }

        inline static std::unordered_map<std::string, ZoomId> const STRING_ZOOM_IDS{
            { "SGFX", ZoomId::SGFX },
            { "MGFX", ZoomId::MGFX },
            { "GFX", ZoomId::GFX }
        };

        static ZoomId StringToZoomId(const std::string& t_zoomString)
        {
            const auto it{ STRING_ZOOM_IDS.find(t_zoomString) };
            return it == STRING_ZOOM_IDS.end() ?
                ZoomId::NOT_ZOOMABLE : it->second;
        }
    };
}
