#include "Input.h"

//-------------------------------------------------
// Singleton
//-------------------------------------------------

sg::Input& sg::Input::GetInstance()
{
    static Input input;
    volatile int dummy{};
    return input;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::Input::OnUpdate()
{
    for (const auto& it : m_keyMap)
    {
        m_previousKeyMap[it.first] = it.second;
    }
}

//-------------------------------------------------
// Keys
//-------------------------------------------------

void sg::Input::KeyPress(const int32_t t_keyId)
{
    m_keyMap[t_keyId] = true;
}

void sg::Input::KeyRelease(const int32_t t_keyId)
{
    m_keyMap[t_keyId] = false;
}

bool sg::Input::IsKeyDown(const int32_t t_keyId)
{
    const auto it{ m_keyMap.find(t_keyId) };
    if (it != m_keyMap.end())
    {
        return it->second;
    }

    return false;
}

bool sg::Input::IsKeyPressed(const int32_t t_keyId)
{
    if (IsKeyDown(t_keyId) == true && WasKeyDown(t_keyId) == false)
    {
        return true;
    }

    return false;
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool sg::Input::WasKeyDown(const int32_t t_keyId)
{
    const auto it{ m_previousKeyMap.find(t_keyId) };
    if (it != m_previousKeyMap.end())
    {
        return it->second;
    }

    return false;
}
