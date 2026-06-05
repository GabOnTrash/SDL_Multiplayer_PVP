#pragma once

#include "../GameState.hpp"

class MenuState : public GameState
{
public:
    void onEnter(GameContext& context) override;
    void onExit(GameContext& context) override;
    void update(float ts, GameContext& context) override;
    void render(GameContext& context) override;
};