#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "vendor/imgui/imgui_impl_sdl.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "Window.h"
#include "Game.h"
#include "OpenGL.h"
#include "Log.h"
#include "SgAssert.h"
#include "SgException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::Window::Window(Game* const t_parentGame)
    : m_parentGame{ t_parentGame }
{
    SG_ASSERT(m_parentGame, "[Window::Window()] Null pointer.");

    Log::SG_LOG_DEBUG("[Window::Window()] Create Window.");
}

sg::Window::~Window()
{
    Log::SG_LOG_DEBUG("[Window::~Window()] Destruct Window.");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_context)
    {
        SDL_GL_DeleteContext(m_context);
    }

    if (m_window)
    {
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::Window::Init()
{
    // init SDL
    Log::SG_LOG_DEBUG("[Window::Init()] Initialize SDL.");
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw SG_EXCEPTION("[Window::Init()] Unable to initialize SDL. Error: " + std::string(reinterpret_cast<const char*>(SDL_GetError())));
    }

    // framebuffer setup
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_parentGame->windowOptions.glMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_parentGame->windowOptions.glMinor);

    // profile
    if (m_parentGame->windowOptions.compatibleProfile)
    {
        Log::SG_LOG_WARN("[Window::Init()] WARNING: Using the OpenGL Compatibility Profile!");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    }
    else
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }

    // debug
    if (m_parentGame->windowOptions.debugContext)
    {
        Log::SG_LOG_WARN("[Window::Init()] WARNING: Using OpenGL in Debug Context!");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    }

    // antialiasing
    if (m_parentGame->windowOptions.antialiasing)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    }

    // create window
    SG_ASSERT(m_parentGame->projectionOptions.width > 0, "[Window::Init()] The width should be greater than 0.");
    SG_ASSERT(m_parentGame->projectionOptions.height > 0, "[Window::Init()] The height should be greater than 0.");

    Log::SG_LOG_DEBUG("[Window::Init()] Initialize SDL Window. Width: {}, Height {}",
        m_parentGame->projectionOptions.width, m_parentGame->projectionOptions.height);

    m_window = SDL_CreateWindow(
        m_parentGame->windowOptions.title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_parentGame->projectionOptions.width,
        m_parentGame->projectionOptions.height,
        SDL_WINDOW_OPENGL
    );

    if (!m_window)
    {
        throw SG_EXCEPTION("[Window::Init()] Window could not be created. Error: " + std::string(reinterpret_cast<const char*>(SDL_GetError())));
    }

    // create context
    Log::SG_LOG_DEBUG("[Window::Init()] Create OpenGL context.");

    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context)
    {
        Log::SG_LOG_FATAL("[Window::Init()] Context could not be created. Error: {}", SDL_GetError());
        throw SG_EXCEPTION("[Window::Init()] Context could not be created.");
    }

    // vsync
    if (m_parentGame->windowOptions.vSync)
    {
        Log::SG_LOG_DEBUG("[Window::Init()] Try to enable vsync.");

        if (SDL_GL_SetSwapInterval(-1) < 0)
        {
            if (SDL_GL_SetSwapInterval(1) < 0)
            {
                Log::SG_LOG_WARN("[Window::Init()] WARNING: Setting the swap interval is not supported.");
            }
        }
    }

    // init Glew
    Log::SG_LOG_DEBUG("[Window::Init()] Initialize GLEW.");

    glewExperimental = GL_TRUE;
    const auto err{ glewInit() };
    if (err != GLEW_OK)
    {
        throw SG_EXCEPTION("[Window::Init()] Unable to initialize GLEW. " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    // register debug callback function
    if (m_parentGame->windowOptions.debugContext)
    {
        int32_t flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            Log::SG_LOG_DEBUG("[Window::Init()] Initialize OpenGL debug output.");

            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(gl_debug_output, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else
        {
            throw SG_EXCEPTION("[Window::Init()] Unable to initialize the OpenGL debug output.");
        }
    }

    // info
    Log::SG_LOG_INFO("OpenGL version: {}", glGetString(GL_VERSION));
    Log::SG_LOG_INFO("GLSL version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
    Log::SG_LOG_INFO("Vendor: {}", glGetString(GL_VENDOR));
    Log::SG_LOG_INFO("Renderer: {}", glGetString(GL_RENDERER));

    // set/update the projection matrix
    UpdateProjectionMatrix();
    UpdateOrthographicProjectionMatrix();

    // init ImGui
    Log::SG_LOG_DEBUG("[Window::Init()] Initialize ImGui.");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ ImGui::GetIO() };
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

//-------------------------------------------------
// Update
//-------------------------------------------------

void sg::Window::OnUpdate() const
{
    SDL_GL_SwapWindow(m_window);
}

void sg::Window::UpdateProjectionMatrix()
{
    m_projectionMatrix = glm::perspectiveFov
    (
        glm::radians(m_parentGame->projectionOptions.fovDeg),
        static_cast<float>(m_parentGame->projectionOptions.width),
        static_cast<float>(m_parentGame->projectionOptions.height),
        m_parentGame->projectionOptions.nearPlane,
        m_parentGame->projectionOptions.farPlane
    );
}

void sg::Window::UpdateOrthographicProjectionMatrix()
{
    m_orthographicProjectionMatrix = glm::ortho
    (
        0.0f,
        static_cast<float>(m_parentGame->projectionOptions.width),
        static_cast<float>(m_parentGame->projectionOptions.height),
        0.0f,
        1.0f,
        -1.0f
    );
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void sg::Window::ImGuiBegin() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
    ImGui::NewFrame();
}

void sg::Window::ImGuiEnd()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
