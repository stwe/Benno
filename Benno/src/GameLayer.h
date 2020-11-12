#pragma once

#include <memory>
#include "Layer.h"

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
    class Game;

    class GameLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        GameLayer() = delete;

        GameLayer(Game* t_parentGame, const std::string& t_name);

        GameLayer(const GameLayer& t_other) = delete;
        GameLayer(GameLayer&& t_other) noexcept = delete;
        GameLayer& operator=(const GameLayer& t_other) = delete;
        GameLayer& operator=(GameLayer&& t_other) noexcept = delete;

        ~GameLayer() = default;

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void OnCreate() override;
        void OnDestruct() override;

        void OnUpdate() override;
        void OnRender() override;
        void OnGuiRender() override;
        void OnSdlEvent(const SDL_Event& t_event) override;
        void OnInput() override;

    protected:

    private:
        std::unique_ptr<file::PaletteFile> m_paletteFile;
        std::unique_ptr<file::BshFile> m_bshFile;
        std::unique_ptr<renderer::MeshRenderer> m_renderer;

        int m_bshIndex{ 5372 };
    };
}
