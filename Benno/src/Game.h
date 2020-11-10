#pragma once

#include <memory>
#include "Config.h"
#include "ImGuiLayer.h"
#include "LayerStack.h"

namespace sg
{
    class Window;
    class Layer;

    class Game
    {
    public:
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

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

    protected:

    private:
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imGuiLayer{ nullptr };
        LayerStack m_layerStack;

        bool m_running{ true };

        //-------------------------------------------------
        // Init && GameLoop
        //-------------------------------------------------

        void Init();
        void GameLoop();

        //-------------------------------------------------
        // Layer
        //-------------------------------------------------

        void PushLayer(Layer* t_layer);
        void PushOverlay(Layer* t_layer);

        void OnEvent();
        bool OnWindowClose();

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input();
        void Update();
        void Render();
    };
}
