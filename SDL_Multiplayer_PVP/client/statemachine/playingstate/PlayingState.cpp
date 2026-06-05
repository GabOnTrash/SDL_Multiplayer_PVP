#include "PlayingState.hpp"

void PlayingState::onEnter(GameContext& context)
{
    playerTexture = IMG_LoadTexture(context.renderer.GetSDLRenderer(), "assets/player/idle.png");

    context.inputManager.BindAction("move_left", SDL_SCANCODE_A);
    context.inputManager.BindAction("move_right", SDL_SCANCODE_D);
    context.inputManager.BindAction("move_up", SDL_SCANCODE_W);
    context.inputManager.BindAction("move_down", SDL_SCANCODE_S);
}

void PlayingState::onExit(GameContext& context)
{
}

void PlayingState::update(float ts, GameContext& context)
{
    if (ts - startTime > 100)
    {
        currentFrame = (currentFrame + 1) % 11;
        startTime = SDL_GetTicks();
    }

    if (context.inputManager.IsActionHeld("move_left"))
    {                                
        playerX -= 5;                
    }                                
    if (context.inputManager.IsActionHeld("move_right"))
    {                                
        playerX += 5;                
    }                                
    if (context.inputManager.IsActionHeld("move_up"))
    {                                
        playerY -= 5;                
    }                                
    if (context.inputManager.IsActionHeld("move_down"))
    {
        playerY += 5;
    }
}

void PlayingState::render(GameContext& context)
{
    SDL_FRect src = {currentFrame * 32.0f, 0.0f, 32.0f, 32.0f}; // Porzione di texture da ritagliare
    SDL_FRect dst = {playerX, playerY, 64.0f, 64.0f};             // Posizione e dimensione sullo schermo

    context.renderer.DrawTexture(playerTexture, &src, &dst);
}
