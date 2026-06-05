#pragma once

#include <stack>
#include <memory>

#include "GameState.hpp"

class StateMachine
{
public:
    void push(std::unique_ptr<GameState> s, GameContext& context)
    {
        if (!states.empty())
            states.top()->onExit(context);
        
        states.push(std::move(s));
        states.top()->onEnter(context);
    }

    void pop(GameContext& context)
    {
        states.top()->onExit(context);
        states.pop();
        
        if (!states.empty())
            states.top()->onEnter(context);
    }

    void update(float ts, GameContext& context)
    {
        if (!states.empty())
            states.top()->update(ts, context);
    }

    void render(GameContext& context)
    {
        if (!states.empty())
            states.top()->render(context);
    }

private:
    std::stack<std::unique_ptr<GameState>> states;
};