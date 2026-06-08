#pragma once

#include "../gamecontext/GameContext.hpp"
#include "../renderer/Renderer.hpp"
#include "../inputmanager/InputManager.hpp"

class GameState
{
public:
    virtual ~GameState() = default;
    
    virtual void onEnter(GameContext& context) {}
    virtual void onExit(GameContext& context) {}
    virtual void update(float ts, GameContext& context) = 0;
    virtual void render(GameContext& context) = 0;
};