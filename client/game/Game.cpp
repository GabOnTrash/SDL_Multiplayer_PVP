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
    uint64_t lastTime = SDL_GetTicks();

    while (true)
    {
        uint64_t currentTime = SDL_GetTicks();
        float ts = static_cast<float>(currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        ts = std::min(ts, 0.1f);

        m_context.inputManager.update(m_context.renderer.GetSDLRenderer());
        m_context.stateMachine.update(ts, m_context);

        m_context.renderer.Clear();
        m_context.stateMachine.render(m_context);
        m_context.renderer.Present();
    }
}

