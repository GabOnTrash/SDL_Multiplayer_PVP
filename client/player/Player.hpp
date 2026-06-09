#pragma once

#include <SDL3/SDL.h>
#include "../renderer/Renderer.hpp"
#include "../gamecontext/GameContext.hpp"
#include "../inputmanager/InputManager.hpp"
#include "../../common/physics/PhysicsBody.hpp"

class Player
{
public:
	Player();
	~Player();
    void update(float ts, GameContext& context);
    void render(Renderer& renderer);

    SDL_Texture*& GetPlayerTexture();
    PhysicsBody*& GetPhysicsBody();

    uint32_t id = 0;

private:
    SDL_Texture* playerTexture;
    PhysicsBody* body = nullptr;

    int currentFrame = 0;
    Uint32 startTime = 0; // Modificato in Uint32, il tipo corretto per SDL_GetTicks()

    int m_numFrames = 11;
    Uint32 m_animSpeedMs = 100; // Millisecondi tra un frame e l'altro
};
