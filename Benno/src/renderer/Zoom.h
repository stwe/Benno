#pragma once

#include <unordered_map>
#include <vector>
#include "SgException.h"

namespace sg::renderer
{
    class Zoom
    {
    public:
        enum class ZoomId
        {
            SGFX, MGFX, GFX, NOT_ZOOMABLE
        };

        inline static std::vector<ZoomId> const ZOOM_TABLE{ ZoomId::SGFX, ZoomId::MGFX, ZoomId::GFX };

        static const std::string& ZoomToString(const ZoomId t_zoomId)
        {
            return ZOOM_STRINGS.find(t_zoomId)->second;
        }

        static ZoomId StringToZoom(const std::string& t_zoomString)
        {
            const auto it{ STRINGS_ZOOM.find(t_zoomString) };
            return it == STRINGS_ZOOM.end() ?
                ZoomId::NOT_ZOOMABLE : it->second;
        }

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Zoom() = delete;

        Zoom(const ZoomId t_zoomId, const int t_xRaster, const int t_yRaster, const int t_elevation,
            const int t_defaultTileWidth, const int t_defaultTileHeight)
            : m_zoomId{ t_zoomId }
            , m_xRaster{ t_xRaster }
            , m_yRaster{ t_yRaster }
            , m_elevation{ t_elevation }
            , m_defaultTileWidth{ t_defaultTileWidth }
            , m_defaultTileHeight{ t_defaultTileHeight }
        {
        }

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] ZoomId GetZoomId() const { return m_zoomId; }
        [[nodiscard]] int GetXRaster() const { return m_xRaster; }
        [[nodiscard]] int GetYRaster() const { return m_yRaster; }
        [[nodiscard]] int GetElevation() const { return m_elevation; }
        [[nodiscard]] int GetDefaultTileWidth() const { return m_defaultTileWidth; }
        [[nodiscard]] int GetDefaultTileHeight() const { return m_defaultTileHeight; }

        [[nodiscard]] int GetTileWidth() const { return m_defaultTileWidth; }
        [[nodiscard]] int GetTileHeight() const
        {
            return m_defaultTileHeight == 31 ? m_defaultTileHeight + 1 : m_defaultTileHeight;
        }

    protected:

    private:
        inline static std::unordered_map<ZoomId, std::string> const ZOOM_STRINGS
        {
            { ZoomId::SGFX, "SGFX" },
            { ZoomId::MGFX, "MGFX" },
            { ZoomId::GFX, "GFX" }
        };

        inline static std::unordered_map<std::string, ZoomId> const STRINGS_ZOOM
        {
            { "SGFX", ZoomId::SGFX },
            { "MGFX", ZoomId::MGFX },
            { "GFX", ZoomId::GFX }
        };

        ZoomId m_zoomId{ ZoomId::NOT_ZOOMABLE };
        int m_xRaster{ 0 };
        int m_yRaster{ 0 };
        int m_elevation{ 0 };
        int m_defaultTileWidth{ 0 };
        int m_defaultTileHeight{ 0 };
    };

    class ZoomFactory
    {
    public:
        static Zoom CreateZoom(const Zoom::ZoomId t_zoomId)
        {
            switch (t_zoomId)
            {
                case Zoom::ZoomId::SGFX:
                    return Zoom(Zoom::ZoomId::SGFX, 8, 4, 4, 16, 8);
                case Zoom::ZoomId::MGFX:
                    return Zoom(Zoom::ZoomId::MGFX, 16, 8, 2, 32, 16);
                case Zoom::ZoomId::GFX:
                    return Zoom(Zoom::ZoomId::GFX, 32, 16, 1, 64, 31);
                case Zoom::ZoomId::NOT_ZOOMABLE:
                default: break;
            }

            throw SG_EXCEPTION("[ZoomFactory::CreateZoom()] Invalid Zoom Id.");
        }

    protected:

    private:

    };
}
