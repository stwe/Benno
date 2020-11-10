#pragma once

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

        explicit Layer(Game* t_parentGame);

        Layer(const Layer& t_other) = delete;
        Layer(Layer&& t_other) noexcept = delete;
        Layer& operator=(const Layer& t_other) = delete;
        Layer& operator=(Layer&& t_other) noexcept = delete;

        virtual ~Layer() = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] Game* GetParentGame() const noexcept;

        //-------------------------------------------------
        // Interface
        //-------------------------------------------------

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnRender() {}
        virtual void OnGuiRender() {}
        virtual void OnEvent() {}

    protected:

    private:
        Game* m_parentGame{ nullptr };
    };
}
