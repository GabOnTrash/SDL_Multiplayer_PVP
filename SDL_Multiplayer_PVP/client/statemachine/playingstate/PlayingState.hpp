#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "../GameState.hpp"

class PlayingState : public GameState
{
public:
    void onEnter(GameContext& context) override;
    void onExit(GameContext& context) override;
    void update(float ts, GameContext& context) override;
    void render(GameContext& context) override;

private:
    SDL_Texture* playerTexture = nullptr; // Texture del giocatore
    int currentFrame = 0;                 // Frame attuale dell'animazione
    int startTime = 0;                    // Tempo di inizio dell'animazione

    int playerX = 100; // Posizione X del giocatore
    int playerY = 100; // Posizione Y del giocatore
};