#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "../GameState.hpp"
#include "../../player/Player.hpp"
#include "../../../common/physics/PhysicsEngine.hpp"

class PlayingState : public GameState
{
public:
    void onEnter(GameContext& context) override;
    void onExit(GameContext& context) override;
    void update(float ts, GameContext& context) override;
    void render(GameContext& context) override;

private:
    Player m_player;

    PhysicsBody* floor = nullptr;
    PhysicsBody* floor2 = nullptr;
    SDL_Texture* txtFloor = nullptr;

    PhysicsEngine m_physicsEngine; // Motore fisico per gestire la fisica del gioco
};