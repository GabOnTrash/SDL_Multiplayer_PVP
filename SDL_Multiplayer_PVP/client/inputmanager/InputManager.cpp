#include "InputManager.hpp"

InputManager::InputManager()
{
    m_currentKeyStates = SDL_GetKeyboardState(&m_numKeys);
    m_previousKeyStates.resize(m_numKeys, false);
}

void InputManager::update(SDL_Renderer* renderer)
{
    SDL_PumpEvents();

    for (int i = 0; i < m_numKeys; ++i)
        m_previousKeyStates[i] = m_currentKeyStates[i];

    m_previousMouseState = m_currentMouseState;

    float physicalX = 0.0f;
    float physicalY = 0.0f;
    m_currentMouseState = SDL_GetMouseState(&physicalX, &physicalY);

    if (renderer)
        SDL_RenderCoordinatesFromWindow(renderer, physicalX, physicalY, &m_mouseX, &m_mouseY);
    else
    {
        m_mouseX = physicalX;
        m_mouseY = physicalY;
    }
}

// keyboard input
bool InputManager::IsKeyPressed(SDL_Scancode key) const
{
    return m_currentKeyStates[key] && !m_previousKeyStates[key];
}

bool InputManager::IsKeyReleased(SDL_Scancode key) const
{
    return !m_currentKeyStates[key] && m_previousKeyStates[key];
}

bool InputManager::IsKeyHeld(SDL_Scancode key) const
{
    return m_currentKeyStates[key];
}

// mouse input
float InputManager::GetMouseX() const
{
    return m_mouseX;
}

float InputManager::GetMouseY() const
{
    return m_mouseY;
}

bool InputManager::IsMouseButtonPressed(int button) const
{
    bool isCurrentDown = (m_currentMouseState & SDL_BUTTON_MASK(button)) != 0;
    bool wasPreviousDown = (m_previousMouseState & SDL_BUTTON_MASK(button)) != 0;

    return isCurrentDown && !wasPreviousDown;
}

bool InputManager::IsMouseButtonReleased(int button) const
{
    bool isCurrentDown = (m_currentMouseState & SDL_BUTTON_MASK(button)) != 0;
    bool wasPreviousDown = (m_previousMouseState & SDL_BUTTON_MASK(button)) != 0;

    return !isCurrentDown && wasPreviousDown;
}

bool InputManager::IsMouseButtonHeld(int button) const
{
    return (m_currentMouseState & SDL_BUTTON_MASK(button)) != 0; // idk how it works really under the hood
}

// action mapping
void InputManager::BindAction(const std::string& actionName, SDL_Scancode key)
{
    m_actionMap[actionName].push_back(key);
}

void InputManager::ClearAction(const std::string& actionName)
{
    m_actionMap[actionName].clear();
}

bool InputManager::IsActionPressed(const std::string& actionName) const
{
    auto it = m_actionMap.find(actionName);
    if (it != m_actionMap.end())
    {
        for (const auto& key : it->second)
        {
            if (IsKeyPressed(key))
                return true;
        }
    }
    return false;
}

bool InputManager::IsActionReleased(const std::string& actionName) const
{
    auto it = m_actionMap.find(actionName);
    if (it != m_actionMap.end())
    {
        for (const auto& key : it->second)
        {
            if (IsKeyReleased(key))
                return true;
        }
    }
    return false;

}

bool InputManager::IsActionHeld(const std::string& actionName) const
{
    auto it = m_actionMap.find(actionName);
    if (it != m_actionMap.end())
    {
        for (const auto& key : it->second)
        {
            if (IsKeyHeld(key))
                return true;
        }
    }
    return false;
}