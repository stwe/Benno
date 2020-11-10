#include <sstream>
#include <SDL2/SDL.h>
#include "Game.h"
#include "Window.h"
#include "Layer.h"
#include "Log.h"
#include "GameLayer.h"

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

    PushLayer(new GameLayer(this));

    m_imGuiLayer = new ImGuiLayer(this);
    PushOverlay(m_imGuiLayer);
}

void sg::Game::GameLoop()
{
    const auto frametime{ static_cast<uint32_t>(1000 / windowOptions.fps) };

    auto lastTime{ SDL_GetTicks() };
    auto timer{ lastTime };

    auto deltaTime = 0;

    auto frames = 0;
    auto updates = 0;

    SDL_Event e;
    while (m_running)
    {
        // measure time
        const auto nowTime{ SDL_GetTicks() };
        deltaTime += (nowTime - lastTime) / frametime;
        lastTime = nowTime;

        // only update at 60 frames per second (default)
        while (deltaTime >= 1000)
        {
            Update();
            updates++;
            deltaTime--;
        }

        // render at maximum possible frames
        Render();
        frames++;

        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }
        }
        Input();

        // reset after one second
        if (SDL_GetTicks() - timer > 1000)
        {
            timer += 1000;

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

        m_window->Update();
    }
}

//-------------------------------------------------
// Layer
//-------------------------------------------------

void sg::Game::PushLayer(Layer* t_layer)
{
    m_layerStack.PushLayer(t_layer);
    t_layer->OnAttach();
}

void sg::Game::PushOverlay(Layer* t_layer)
{
    m_layerStack.PushOverlay(t_layer);
    t_layer->OnAttach();
}

void sg::Game::OnEvent()
{
    for (auto it{ m_layerStack.rbegin() }; it != m_layerStack.rend(); ++it)
    {
        (*it)->OnEvent();
    }
}

bool sg::Game::OnWindowClose()
{
    m_running = false;

    return true;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::Game::Input()
{
    for (auto* layer : m_layerStack)
    {
        layer->OnEvent();
    }
}

void sg::Game::Update()
{
    for (auto* layer : m_layerStack)
    {
        layer->OnUpdate();
    }
}

void sg::Game::Render()
{
    for (auto* layer : m_layerStack)
    {
        layer->OnRender();
    }

    m_imGuiLayer->Begin();
    for (auto* layer : m_layerStack)
    {
        layer->OnGuiRender();
    }
    m_imGuiLayer->End();
}
