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
        bool vSync{ false };
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
        renderer::Zoom::ZoomId currentZoomId{ renderer::Zoom::ZoomId::SGFX };
#if defined(_WIN64)
        std::string resourcePath{ "E:\\Anno" };
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 8)
        std::string resourcePath{ "/home/steffen/Anno" };
#else
        #error Unsupported platform or compiler!
#endif
    };
}
