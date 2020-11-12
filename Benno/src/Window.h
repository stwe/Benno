#pragma once

#include <glm/mat4x4.hpp>

struct SDL_Window;

namespace sg
{
    class Game;

    class Window
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Window() = delete;

        explicit Window(Game* t_parentGame);

        Window(const Window& t_other) = delete;
        Window(Window&& t_other) noexcept = delete;
        Window& operator=(const Window& t_other) = delete;
        Window& operator=(Window&& t_other) noexcept = delete;

        ~Window();

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] auto* GetSdlWindow() const noexcept { return m_window; }
        [[nodiscard]] auto* GetContext() const noexcept { return m_context; }
        [[nodiscard]] glm::mat4 GetProjectionMatrix() const { return m_projectionMatrix; }
        [[nodiscard]] glm::mat4 GetOrthographicProjectionMatrix() const { return m_orthographicProjectionMatrix; }

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void OnUpdate() const;
        void UpdateProjectionMatrix();
        void UpdateOrthographicProjectionMatrix();

        //-------------------------------------------------
        // ImGui
        //-------------------------------------------------

        void ImGuiBegin() const;
        static void ImGuiEnd();

    protected:

    private:
        Game* m_parentGame{ nullptr };

        SDL_Window* m_window{ nullptr };
        void* m_context{ nullptr };

        /**
         * @brief The perspective projection matrix.
         *        Updated with UpdateProjectionMatrix().
         */
        glm::mat4 m_projectionMatrix{ glm::mat4(1.0f) };

        /**
         * @brief The orthographic projection matrix.
         *        Updated with UpdateOrthographicProjectionMatrix().
         */
        glm::mat4 m_orthographicProjectionMatrix{ glm::mat4(1.0f) };
    };
}
