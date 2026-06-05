#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

class InputManager
{
public:
    InputManager();
    
    void update(SDL_Renderer* renderer);

    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsKeyReleased(SDL_Scancode key) const;
    bool IsKeyHeld(SDL_Scancode key) const;

    float GetMouseX() const;
    float GetMouseY() const;
    bool IsMouseButtonPressed(int button) const;
    bool IsMouseButtonReleased(int button) const;
    bool IsMouseButtonHeld(int button) const;

    void BindAction(const std::string& actionName, SDL_Scancode key);
    void ClearAction(const std::string& actionName);
    bool IsActionPressed(const std::string& actionName) const;
    bool IsActionReleased(const std::string& actionName) const;
    bool IsActionHeld(const std::string& actionName) const;

private:
    // keyboard state
    const bool* m_currentKeyStates;
    std::vector<bool> m_previousKeyStates;
    int m_numKeys;

    // mouse state
    float m_mouseX = 0;
    float m_mouseY = 0;
    SDL_MouseButtonFlags m_currentMouseState;
    SDL_MouseButtonFlags m_previousMouseState;

    // action map
    std::unordered_map<std::string, std::vector<SDL_Scancode>> m_actionMap;
};