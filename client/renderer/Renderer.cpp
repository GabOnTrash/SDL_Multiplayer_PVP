#include "Renderer.hpp"

Renderer::Renderer() 
    : m_window(nullptr), m_renderer(nullptr)
{
}

Renderer::~Renderer()
{
    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);
    if (m_window)
        SDL_DestroyWindow(m_window);

    SDL_Quit();
}

bool Renderer::Init(const WindowConfig& config)
{
    m_config = config;
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(display);
    int w, h;
    if (mode)
    {
        w = mode->w;
        h = mode->h;
    }

    SDL_WindowFlags windowFlags = (m_config.fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | 
                                  (m_config.resizeable ? SDL_WINDOW_RESIZABLE : 0);

    // window creation
    m_window = SDL_CreateWindow(m_config.title.c_str(), w, h, windowFlags);
    if (!m_window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // renderer creation
    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    // logical presentation
    if (!SDL_SetRenderLogicalPresentation(m_renderer, m_config.virtualWidth, m_config.virtualHeight,
                                          SDL_LOGICAL_PRESENTATION_LETTERBOX))
    {
        SDL_Log("Failed to set logical presentation: %s", SDL_GetError());
        return false;
    }

    if (!SDL_SetRenderVSync(m_renderer,m_config.vsync))
        SDL_Log("Failed to set VSync: %s", SDL_GetError());

    return true;
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::SetCameraTarget(const Box& target)
{
    m_camera.size = {
        static_cast<float>(m_config.virtualWidth), 
        static_cast<float>(m_config.virtualHeight)
    };
    m_camera.Follow(target);
}

void Renderer::DrawTexture(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect)
{
    if (!texture)
    {
        SDL_Log("Texture is null");
        return;
    }

    SDL_FRect adjustedDstRect = *dstRect; 
    adjustedDstRect.x -= m_camera.position.x;
    adjustedDstRect.y -= m_camera.position.y;
    SDL_RenderTexture(m_renderer, texture, srcRect, (const SDL_FRect*) &adjustedDstRect);
}

void Renderer::DrawTextureStatic(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect)
{
    if (!texture)
    {
        SDL_Log("Texture is null");
        return;
    }

    SDL_RenderTexture(m_renderer, texture, srcRect, dstRect);
}


void Renderer::Present()
{
    SDL_RenderPresent(m_renderer);
}

SDL_Renderer* Renderer::GetSDLRenderer() const
{
    return m_renderer;
}

WindowConfig Renderer::GetWindowConfig() const
{
    return m_config;
}
