#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

struct WindowConfig
{
    std::string title;
    int virtualWidth;
    int virtualHeight;
    bool vsync;
    bool resizeable;
    bool fullscreen;
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Init(const WindowConfig& config);
    
    void Clear();
    void DrawTexture(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect);
    void Present();

    SDL_Renderer* GetSDLRenderer() const;

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};