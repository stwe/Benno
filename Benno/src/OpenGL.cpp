#include <iostream>
#include "OpenGL.h"

//-------------------------------------------------
// Debug output
//-------------------------------------------------

void APIENTRY gl_debug_output(
    const uint32_t t_source,
    const uint32_t t_type,
    const uint32_t t_id,
    const uint32_t t_severity,
    int32_t t_length,
    const char* t_message,
    const void* t_userParam
)
{
    // ignore non-significant error/warning codes
    if (t_id == 131169 || t_id == 131185 || t_id == 131218 || t_id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << t_id << "): " << t_message << std::endl;

    switch (t_source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        default:;
    } std::cout << std::endl;

    switch (t_type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        default:;
    } std::cout << std::endl;

    switch (t_severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        default:;
    } std::cout << std::endl;

    std::cout << std::endl;
}

//-------------------------------------------------
// OpenGL states
//-------------------------------------------------

void sg::OpenGL::SetClearColor(const float t_red, const float t_green, const float t_blue)
{
    glClearColor(t_red, t_green, t_blue, 1.0f);
}

void sg::OpenGL::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void sg::OpenGL::EnableAlphaBlending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void sg::OpenGL::DisableBlending()
{
    glDisable(GL_BLEND);
}

void sg::OpenGL::EnableWireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void sg::OpenGL::DisableWireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
