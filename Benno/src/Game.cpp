#include <sstream>
#include <SDL2/SDL.h>
#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Layer.h"
#include "Log.h"
#include "GameLayer.h"
#include "vendor/imgui/imgui.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::Game::Game()
{
    Log::SG_LOG_DEBUG("[Game::Game()] Create Game.");
}

sg::Game::~Game()
{
    Log::SG_LOG_DEBUG("[Game::~Game()] Destruct Game.");
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

const sg::Window& sg::Game::GetWindow() const noexcept
{
    return *m_window;
}

const sg::file::Files& sg::Game::GetFiles() const noexcept
{
    return m_files;
}

//-------------------------------------------------
// Run
//-------------------------------------------------

void sg::Game::Run()
{
    Log::SG_LOG_DEBUG("[Game::Run()] Game is started.");

    Init();
    GameLoop();
}

//-------------------------------------------------
// Init && GameLoop
//-------------------------------------------------

void sg::Game::Init()
{
    m_window = std::make_unique<Window>(this);
    m_window->Init();

    AddLayer(new GameLayer(this, "GameScreen"));
}

void sg::Game::GameLoop()
{
    auto accumulator{ 0ns };
    auto timeStart{ Clock::now() };
    auto resetTime{ timeStart };

    auto frames{ 0 };
    auto updates{ 0 };

    while (m_running)
    {
        const auto currentTime{ Clock::now() };
        auto frameTime{ currentTime - timeStart };
        timeStart = currentTime;
        accumulator += std::chrono::duration_cast<std::chrono::nanoseconds>(frameTime);

        // Update
        while (accumulator > TIMESTEP)
        {
            Input::GetInstance().OnUpdate();
            OnUpdate();

            accumulator -= TIMESTEP;
            updates++;
        }

        // Render
        OnRender();
        frames++;

        // Events / Input
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            OnSdlEvent(event);
        }
        OnInput();

        // Info
        if (std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - resetTime) > 1s)
        {
            resetTime = currentTime;

            if (windowOptions.printFrameRate)
            {
                std::stringstream ss;

#ifdef SG_DEBUG_BUILD
                ss << windowOptions.title << " [DEBUG BUILD] " << "   |   Fps: " << frames << "   |   Updates: " << updates;
#else
                ss << windowOptions.title << " [RELEASE BUILD] " << "   |   Fps: " << frames << "   |   Updates: " << updates;
#endif

                SDL_SetWindowTitle(m_window->GetSdlWindow(), ss.str().c_str());
            }

            updates = 0;
            frames = 0;
        }

        m_window->OnUpdate();
    }
}

//-------------------------------------------------
// Layer
//-------------------------------------------------

void sg::Game::AddLayer(Layer* t_layer)
{
    m_layerList.Add(t_layer);
    t_layer->OnCreate();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::Game::OnUpdate()
{
    for (auto* layer : m_layerList)
    {
        layer->OnUpdate();
    }
}

void sg::Game::OnRender()
{
    for (auto* layer : m_layerList)
    {
        layer->OnRender();
    }

    m_window->ImGuiBegin();

    ImGui::Begin("Game Debug");

    ImGui::Text("Press the Escape key to exit.");
    ImGui::Spacing();
    ImGui::Text("Mouse x: %f", Input::GetInstance().GetMousePosition().x);
    ImGui::Text("Mouse y: %f", Input::GetInstance().GetMousePosition().y);

    ImGui::End();

    for (auto* layer : m_layerList)
    {
        layer->OnGuiRender();
    }

    m_window->ImGuiEnd();
}

void sg::Game::OnSdlEvent(const SDL_Event& t_event)
{
    switch (t_event.type)
    {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_MOUSEMOTION:
            Input::GetInstance().SetMousePosition(static_cast<float>(t_event.motion.x), static_cast<float>(t_event.motion.y));
            break;
        case SDL_KEYDOWN:
            Input::GetInstance().KeyPress(t_event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            Input::GetInstance().KeyRelease(t_event.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            Input::GetInstance().KeyPress(t_event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            Input::GetInstance().KeyRelease(t_event.button.button);
            break;
        default:
            break;
    }

    for (auto* layer : m_layerList)
    {
        layer->OnSdlEvent(t_event);
    }
}

void sg::Game::OnInput()
{
    if (Input::GetInstance().IsKeyPressed(SDLK_ESCAPE))
    {
        m_running = false;
    }

    if (Input::GetInstance().IsKeyDown(SDL_BUTTON_RIGHT))
    {
        Log::SG_LOG_DEBUG("Right mouse button pressed.");
    }

    for (auto* layer : m_layerList)
    {
        layer->OnInput();
    }
}
