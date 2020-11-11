#pragma once

#include "Layer.h"

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
        void OnEvent() override;

    protected:

    private:

    };
}
