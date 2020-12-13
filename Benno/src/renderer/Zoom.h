#pragma once

#include <unordered_map>

namespace sg::renderer
{
    class Zoom
    {
    public:
        enum class ZoomId
        {
            SGFX, MGFX, GFX
        };

        inline static const char* zoomMenuItems[] = { "SGFX", "MGFX", "GFX" };

        Zoom() = default;

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
        ZoomId m_zoomId{ ZoomId::GFX };
        int m_xRaster{ 0 };
        int m_yRaster{ 0 };
        int m_elevation{ 0 };
        int m_defaultTileWidth{ 0 };
        int m_defaultTileHeight{ 0 };
    };

    class ZoomFactory
    {
    public:
        ZoomFactory()
        {
            m_zooms.emplace(Zoom::ZoomId::SGFX, Zoom(Zoom::ZoomId::SGFX, 8, 4, 4, 16, 8 ));
            m_zooms.emplace(Zoom::ZoomId::MGFX, Zoom(Zoom::ZoomId::MGFX, 16, 8, 2, 32, 16));
            m_zooms.emplace(Zoom::ZoomId::GFX, Zoom(Zoom::ZoomId::GFX, 32, 16, 1, 64, 31));
        }

        [[nodiscard]] const std::unordered_map<Zoom::ZoomId, Zoom>& GetZooms() const noexcept
        {
            return m_zooms;
        }

    protected:

    private:
        std::unordered_map<Zoom::ZoomId, Zoom> m_zooms;
    };
}
