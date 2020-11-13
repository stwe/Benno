#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>

namespace sg
{
    class Input
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Input(const Input& t_other) = delete;
        Input(Input&& t_other) noexcept = delete;
        Input& operator=(const Input& t_other) = delete;
        Input& operator=(Input&& t_other) noexcept = delete;

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] glm::vec2 GetMousePosition() const noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetMousePosition(float t_x, float t_y) noexcept;

        //-------------------------------------------------
        // Singleton
        //-------------------------------------------------

        static Input& GetInstance();

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void OnUpdate();

        //-------------------------------------------------
        // Keys
        //-------------------------------------------------

        void KeyPress(int32_t t_keyId);
        void KeyRelease(int32_t t_keyId);

        bool IsKeyDown(int32_t t_keyId);
        bool IsKeyPressed(int32_t t_keyId);

        //-------------------------------------------------
        // Mouse
        //-------------------------------------------------

    protected:

    private:
        std::unordered_map<int32_t, bool> m_keyMap;
        std::unordered_map<int32_t, bool> m_previousKeyMap;

        glm::vec2 m_mousePosition{ 0.0f, 0.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Input() = default;
        ~Input() = default;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        bool WasKeyDown(int32_t t_keyId);
    };
}
