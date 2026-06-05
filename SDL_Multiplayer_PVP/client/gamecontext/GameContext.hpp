#pragma once

class Renderer;
class StateMachine;
class InputManager;

struct GameContext
{
    GameContext(Renderer& renderer, StateMachine& stateMachine, InputManager& inputManager/*, AssetManager* assetManager*/)
        : renderer(renderer), stateMachine(stateMachine), inputManager(inputManager)/*, assetManager(assetManager)*/
    {
    }

    Renderer& renderer;
    StateMachine& stateMachine;
    InputManager& inputManager;
};