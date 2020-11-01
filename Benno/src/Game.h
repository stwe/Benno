#pragma once

#include <memory>
#include "Config.h"

namespace sg::file
{
    class PaletteFile;
}

namespace sg
{
    class Window;

    class Game
    {
    public:
        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        WindowOptions windowOptions{};
        ProjectionOptions projectionOptions{};

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
        // Run
        //-------------------------------------------------

        void Run();

    protected:

    private:
        std::unique_ptr<Window> m_window;
        std::unique_ptr<file::PaletteFile> m_paletteFile;

        bool m_quit{ false };

        //-------------------------------------------------
        // Init && GameLoop
        //-------------------------------------------------

        void Init();
        void GameLoop();

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input();
        void Update();
        void Render();
    };
}
