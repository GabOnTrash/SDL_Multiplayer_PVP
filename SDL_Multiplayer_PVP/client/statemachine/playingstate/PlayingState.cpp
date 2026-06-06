#include "PlayingState.hpp"

void PlayingState::onEnter(GameContext& context)
{
    m_player.GetPlayerTexture() = IMG_LoadTexture(context.renderer.GetSDLRenderer(), "assets/player/idle.png");
    txtFloor = IMG_LoadTexture(context.renderer.GetSDLRenderer(), "assets/Terrain.png");

    context.inputManager.BindAction("move_left", SDL_SCANCODE_A);
    context.inputManager.BindAction("move_right", SDL_SCANCODE_D);
    context.inputManager.BindAction("move_up", SDL_SCANCODE_SPACE);

    m_player.GetPhysicsBody() = new PhysicsBody();
    m_player.GetPhysicsBody()->bounds.pos = {100.0f, 100.0f};
    m_player.GetPhysicsBody()->bounds.size = {32.0f, 32.0f};
    m_player.GetPhysicsBody()->isStatic = false;

    m_physicsEngine.AddBody(m_player.GetPhysicsBody());

    floor = new PhysicsBody();
    floor->bounds.pos = {0.0f, 2110.0f};
    floor->bounds.size = {3840, 50.0f};
    floor->isStatic = true; 

    m_physicsEngine.AddBody(floor);
}

void PlayingState::onExit(GameContext& context)
{
}

void PlayingState::update(float ts, GameContext& context)
{
   m_physicsEngine.update(ts);
   m_player.update(ts, context);
}

void PlayingState::render(GameContext& context)
{
    context.renderer.SetCameraTarget(m_player.GetPhysicsBody()->bounds);

    SDL_FRect src = {96, 0, 48, 48};
    SDL_FRect dst = {
        floor->bounds.pos.x, 
        floor->bounds.pos.y, 
        floor->bounds.size.x, 
        floor->bounds.size.y
    };
    context.renderer.DrawTexture(txtFloor, &src, &dst);

    m_player.render(context.renderer);
}
