#pragma once

#include <string>
#include "renderer/Zoom.h"

namespace sg
{
    //-------------------------------------------------
    // Window options
    //-------------------------------------------------

    struct WindowOptions
    {
        std::string title{ "BennoSandbox" };
        bool compatibleProfile{ false };
        bool debugContext{ true };
        bool antialiasing{ true };
        bool printFrameRate{ true };
        int glMajor{ 3 };
        int glMinor{ 3 };
        double fps{ 60.0 };
    };

    //-------------------------------------------------
    // Projection options
    //-------------------------------------------------

    struct ProjectionOptions
    {
        float fovDeg{ 70.0f };
        int width{ 1024 };
        int height{ 768 };
        float nearPlane{ 0.1f };
        float farPlane{ 5000.0f };
    };

    //-------------------------------------------------
    // Game options
    //-------------------------------------------------

    struct GameOptions
    {
        renderer::Zoom::ZoomId currentZoomId{ renderer::Zoom::ZoomId::GFX };
        std::string resourcePath{ "E:\\Anno" };
    };
}
