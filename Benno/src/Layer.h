#pragma once

#include <string>

union SDL_Event;

namespace sg
{
    class Game;

    class Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Layer() = delete;

        Layer(Game* t_parentGame, const std::string& t_name);

        Layer(const Layer& t_other) = delete;
        Layer(Layer&& t_other) noexcept = delete;
        Layer& operator=(const Layer& t_other) = delete;
        Layer& operator=(Layer&& t_other) noexcept = delete;

        virtual ~Layer();

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] Game* GetParentGame() const noexcept;
        [[nodiscard]] const std::string& GetName() const noexcept;

        //-------------------------------------------------
        // Interface
        //-------------------------------------------------

        virtual void OnCreate() {}
        virtual void OnDestruct() {}

        virtual void OnUpdate() {}
        virtual void OnRender() {}
        virtual void OnGuiRender() {}
        virtual void OnSdlEvent(const SDL_Event& t_event) {}
        virtual void OnInput() {}

    protected:
        Game* m_parentGame{ nullptr };
        std::string m_name;

    private:

    };
}
