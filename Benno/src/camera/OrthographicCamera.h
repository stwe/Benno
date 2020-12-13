#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <physics/Aabb.h>

namespace sg
{
    class Layer;
}

namespace sg::camera
{
    class OrthographicCamera
    {
    public:
        enum class Direction { LEFT, RIGHT, UP, DOWN };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        OrthographicCamera() = delete;

        explicit OrthographicCamera(Layer* t_parentLayer);
        OrthographicCamera(Layer* t_parentLayer, const glm::vec2& t_position);

        OrthographicCamera(const OrthographicCamera& t_other) = delete;
        OrthographicCamera(OrthographicCamera&& t_other) noexcept = delete;
        OrthographicCamera& operator=(const OrthographicCamera& t_other) = delete;
        OrthographicCamera& operator=(OrthographicCamera&& t_other) noexcept = delete;

        ~OrthographicCamera();

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        glm::vec2& GetPosition() noexcept;
        [[nodiscard]] const glm::vec2& GetPosition() const noexcept;
        float& GetCameraVelocity() noexcept;
        [[nodiscard]] glm::mat4 GetViewMatrix() const noexcept;
        [[nodiscard]] glm::mat4 GetViewProjectionMatrix() const noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPosition(const glm::vec2& t_position) noexcept;
        void SetCameraVelocity(float t_cameraVelocity) noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void OnUpdate(float t_dt);

        //-------------------------------------------------
        // Physics
        //-------------------------------------------------

        [[nodiscard]] physics::Aabb GetCurrentAabb() const noexcept;

    protected:

    private:
        Layer* m_parentLayer{ nullptr };
        glm::vec2 m_position{ 0.0f };
        float m_cameraVelocity{ 2.0f };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void ProcessKeyboard(Direction t_direction, float t_dt);
    };
}
