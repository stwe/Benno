#pragma once

#include <memory>
#include "Layer.h"

namespace sg::data
{
    class HousesJsonFile;
}

namespace sg::file
{
    class PaletteFile;
    class BshFile;
    class GamFile;
}

namespace sg
{
    class Game;

    class GameLayer : public Layer
    {
    public:
        static constexpr auto WORLD_WIDTH{ 500 };
        static constexpr auto WORLD_HEIGHT{ 350 };

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
        std::shared_ptr<data::HousesJsonFile> m_housesJsonFile;
        std::unique_ptr<file::PaletteFile> m_paletteFile;
        std::shared_ptr<file::BshFile> m_bshFile;
        std::unique_ptr<file::GamFile> m_gamFile;
    };
}
