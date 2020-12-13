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
        const bool compatibleProfile{ false };
        const bool debugContext{ true };
        const bool antialiasing{ true };
        bool printFrameRate{ true };
        const int glMajor{ 3 };
        const int glMinor{ 3 };
        const bool vSync{ false };
    };

    //-------------------------------------------------
    // Projection options
    //-------------------------------------------------

    struct ProjectionOptions
    {
        const float fovDeg{ 70.0f };
        const int width{ 1280 };
        const int height{ 1024 };
        const float nearPlane{ 0.1f };
        const float farPlane{ 5000.0f };
    };

    //-------------------------------------------------
    // Game options
    //-------------------------------------------------

    struct GameOptions
    {
        const renderer::Zoom::ZoomId initialZoomId{ renderer::Zoom::ZoomId::GFX };
        const std::string housesJsonFilePath{ "res/data/houses.json" };

#if defined(_WIN64)
        const std::string resourcePath{ "E:\\Anno" };
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 8)
        const std::string resourcePath{ "/home/steffen/Anno" };
#else
        #error Unsupported platform or compiler!
#endif
    };
}
