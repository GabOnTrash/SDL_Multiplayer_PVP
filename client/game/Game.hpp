#pragma once

#include "../gamecontext/GameContext.hpp"
#include "../renderer/Renderer.hpp"
#include "../statemachine/StateMachine.hpp"
#include "../inputmanager/InputManager.hpp"

#include "../statemachine/menustate/MenuState.hpp"
#include "../statemachine/playingstate/PlayingState.hpp"

class Game
{
public:
    Game(const WindowConfig& config);
    ~Game();

    bool Init();
    void Run();

private:
    WindowConfig m_config;

    Renderer m_renderer;
    StateMachine m_stateMachine;
    InputManager m_inputManager;

    GameContext m_context;
};