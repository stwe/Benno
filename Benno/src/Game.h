#pragma once

#include <memory>
#include "Config.h"

namespace sg::file
{
    class PaletteFile;
    class BshFile;
}

namespace sg::renderer
{
    class MeshRenderer;
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
        std::unique_ptr<file::BshFile> m_bshFile;
        std::unique_ptr<renderer::MeshRenderer> m_renderer;

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
