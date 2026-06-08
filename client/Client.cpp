#include "game/Game.hpp"

int main()
{
    WindowConfig config;
    config.title = "SDL Multiplayer PVP";
    config.virtualWidth = 3840;
    config.virtualHeight = 2160;
    config.vsync = true;
    config.resizeable = false;
    config.fullscreen = true;

    Game game(config);
    
    if (game.Init())
        game.Run();
    else
    {
        SDL_Log("Cannot start the game, errors occurred.");
        return -1;
    }

	return 0;
}
