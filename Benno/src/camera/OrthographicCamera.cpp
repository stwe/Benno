#include <SDL_keycode.h>
#include <glm/gtc/matrix_transform.hpp>
#include "OrthographicCamera.h"
#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Layer.h"
#include "SgAssert.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::camera::OrthographicCamera::OrthographicCamera(Layer* t_parentLayer)
    : m_parentLayer{ t_parentLayer }
{
    SG_ASSERT(t_parentLayer, "[OrthographicCamera::OrthographicCamera()] Null pointer.");

    Log::SG_LOG_DEBUG("[OrthographicCamera::OrthographicCamera()] Create OrthographicCamera.");
}

sg::camera::OrthographicCamera::OrthographicCamera(Layer* t_parentLayer, const glm::vec2& t_position)
    : OrthographicCamera{ t_parentLayer }
{
    m_position = t_position;
}

sg::camera::OrthographicCamera::~OrthographicCamera()
{
    Log::SG_LOG_DEBUG("[OrthographicCamera::~OrthographicCamera()] Destruct OrthographicCamera.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

glm::vec2& sg::camera::OrthographicCamera::GetPosition() noexcept
{
    return m_position;
}

const glm::vec2& sg::camera::OrthographicCamera::GetPosition() const noexcept
{
    return m_position;
}

glm::mat4 sg::camera::OrthographicCamera::GetViewMatrix() const noexcept
{
    auto transformMatrix{ glm::mat4(1.0f) };
    transformMatrix = translate(transformMatrix, glm::vec3(m_position.x, m_position.y, 0.0f));

    return inverse(transformMatrix);
}

glm::mat4 sg::camera::OrthographicCamera::GetViewProjectionMatrix() const noexcept
{
    return m_parentLayer->GetParentGame()->GetWindow().GetOrthographicProjectionMatrix() * GetViewMatrix();
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::camera::OrthographicCamera::SetPosition(const glm::vec2& t_position) noexcept
{
    m_position = t_position;
}

void sg::camera::OrthographicCamera::SetCameraVelocity(const float t_cameraVelocity) noexcept
{
    m_cameraVelocity = t_cameraVelocity;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::camera::OrthographicCamera::OnUpdate(const float t_dt)
{
    if (Input::GetInstance().IsKeyPressed(SDLK_w) || Input::GetInstance().IsKeyPressed(SDLK_UP))
    {
        ProcessKeyboard(Direction::UP, t_dt);
    }

    if (Input::GetInstance().IsKeyPressed(SDLK_s) || Input::GetInstance().IsKeyPressed(SDLK_DOWN))
    {
        ProcessKeyboard(Direction::DOWN, t_dt);
    }

    if (Input::GetInstance().IsKeyPressed(SDLK_a) || Input::GetInstance().IsKeyPressed(SDLK_LEFT))
    {
        ProcessKeyboard(Direction::LEFT, t_dt);
    }

    if (Input::GetInstance().IsKeyPressed(SDLK_d) || Input::GetInstance().IsKeyPressed(SDLK_RIGHT))
    {
        ProcessKeyboard(Direction::RIGHT, t_dt);
    }
}

//-------------------------------------------------
// Physics
//-------------------------------------------------

sg::physics::Aabb sg::camera::OrthographicCamera::GetCurrentAabb() const noexcept
{
    return {
        m_position,
        glm::vec2(
            static_cast<float>(m_parentLayer->GetParentGame()->projectionOptions.width),
            static_cast<float>(m_parentLayer->GetParentGame()->projectionOptions.height)
        )
    };
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::camera::OrthographicCamera::ProcessKeyboard(const Direction t_direction, const float t_dt)
{
    const auto velocity{ m_cameraVelocity * t_dt };

    const auto right{ glm::vec2(0.0f, 1.0f) * velocity };
    const auto left{ glm::vec2(1.0f, 0.0f) * velocity };

    if (t_direction == Direction::UP)
    {
        m_position += right;
    }

    if (t_direction == Direction::DOWN)
    {
        m_position -= right;
    }

    if (t_direction == Direction::LEFT)
    {
        m_position -= left;
    }

    if (t_direction == Direction::RIGHT)
    {
        m_position += left;
    }
}
