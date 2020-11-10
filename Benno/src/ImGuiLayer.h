#pragma once

#include "Layer.h"

namespace sg
{
    class Game;

    class ImGuiLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ImGuiLayer() = delete;

        explicit ImGuiLayer(Game* t_parentGame);

        ImGuiLayer(const ImGuiLayer& t_other) = delete;
        ImGuiLayer(ImGuiLayer&& t_other) noexcept = delete;
        ImGuiLayer& operator=(const ImGuiLayer& t_other) = delete;
        ImGuiLayer& operator=(ImGuiLayer&& t_other) noexcept = delete;

        ~ImGuiLayer() = default;

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent() override;

        //-------------------------------------------------
        // Frame
        //-------------------------------------------------

        void Begin();
        void End();

    protected:

    private:

    };
}
