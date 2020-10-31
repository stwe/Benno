#pragma once

#ifdef _WIN64
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

    #include <Windows.h>
#endif

//-------------------------------------------------
// Glew
//-------------------------------------------------

#include <GL/glew.h>

#ifndef _WIN64
    #define APIENTRY
#endif

//-------------------------------------------------
// SDL2
//-------------------------------------------------

#include <SDL2/SDL.h>

//-------------------------------------------------
// Debug output
//-------------------------------------------------

void APIENTRY gl_debug_output(
    uint32_t t_source,
    uint32_t t_type,
    uint32_t t_id,
    uint32_t t_severity,
    int32_t t_length,
    const char* t_message,
    const void* t_userParam
);

//-------------------------------------------------
// OpenGL States
//-------------------------------------------------

namespace sg
{
    class OpenGL
    {
    public:
        static void SetClearColor(float t_red, float t_green, float t_blue);
        static void Clear();

        static void EnableAlphaBlending();
        static void DisableBlending();

    protected:

    private:

    };
}
