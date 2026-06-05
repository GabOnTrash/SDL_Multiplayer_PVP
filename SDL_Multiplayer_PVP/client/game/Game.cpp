#include "Game.hpp"

Game::Game(const WindowConfig& config) 
    : m_config(config), m_context(m_renderer, m_stateMachine, m_inputManager)
{
}
Game::~Game()
{
}

bool Game::Init()
{
	return m_context.renderer.Init(m_config);
}

void Game::Run()
{
    m_context.stateMachine.push(std::make_unique<PlayingState>(), m_context);

    while (true)
    {
        uint32_t ts = SDL_GetTicks();
        
        m_context.inputManager.update(m_context.renderer.GetSDLRenderer());
        m_context.stateMachine.update(ts, m_context);


        m_context.renderer.Clear();
        m_context.stateMachine.render(m_context);
        m_context.renderer.Present();
    }
}