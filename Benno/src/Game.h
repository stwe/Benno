#pragma once

#include <chrono>
#include <memory>
#include "Config.h"
#include "LayerList.h"
#include "file/Files.h"

union SDL_Event;

namespace sg
{
    class Window;
    class Layer;

    using namespace std::literals::chrono_literals;

    class Game
    {
    public:
        using Clock = std::chrono::high_resolution_clock;

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        WindowOptions windowOptions{};
        ProjectionOptions projectionOptions{};
        GameOptions gameOptions{};

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Game();

        Game(const Game& t_other) = delete;
        Game(Game&& t_other) noexcept = delete;
        Game& operator=(const Game& t_other) = delete;
        Game& operator=(Game&& t_other) noexcept = delete;

        ~Game();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] const Window& GetWindow() const noexcept;
        [[nodiscard]] const file::Files& GetFiles() const noexcept;

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

    protected:

    private:
        static constexpr std::chrono::nanoseconds TIMESTEP{ 16ms };

        std::unique_ptr<Window> m_window;
        LayerList m_layerList;
        file::Files m_files{ gameOptions.resourcePath };

        bool m_running{ true };

        //-------------------------------------------------
        // Init && GameLoop
        //-------------------------------------------------

        void Init();
        void GameLoop();

        //-------------------------------------------------
        // Layer
        //-------------------------------------------------

        void AddLayer(Layer* t_layer);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void OnUpdate();
        void OnRender();
        void OnSdlEvent(const SDL_Event& t_event);
        void OnInput();
    };
}
