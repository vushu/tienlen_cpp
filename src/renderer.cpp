#include "tienlen/renderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace tienlen
{

Renderer &Renderer::getInstance()
{
    static Renderer instance;
    return instance;
}

Renderer::Renderer()
    : window(nullptr), renderer(nullptr), initialized(false)
{
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::init(const std::string &title, int width, int height, bool resizable)
{
    if (initialized)
    {
        SDL_Log("Renderer already initialized");
        return false;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (!TTF_Init())
    {
        SDL_Log("Failed to initialize SDL_ttf: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    Uint32 flags = resizable ? SDL_WINDOW_RESIZABLE : 0;

    if (!SDL_CreateWindowAndRenderer(title.c_str(), width, height, flags, &window, &renderer))
    {
        SDL_Log("Failed to create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    initialized = true;
    return true;
}

void Renderer::cleanup()
{
    if (initialized)
    {
        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        TTF_Quit();
        SDL_Quit();
        initialized = false;
    }
}

void Renderer::clear(float r, float g, float b, float a)
{
    if (!initialized)
        return;

    SDL_SetRenderDrawColorFloat(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

void Renderer::present()
{
    if (!initialized)
        return;

    SDL_RenderPresent(renderer);
}

void Renderer::setDrawColor(float r, float g, float b, float a)
{
    if (!initialized)
        return;

    SDL_SetRenderDrawColorFloat(renderer, r, g, b, a);
}

void Renderer::drawPoint(int x, int y)
{
    if (!initialized)
        return;

    SDL_RenderPoint(renderer, (float)x, (float)y);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2)
{
    if (!initialized)
        return;

    SDL_RenderLine(renderer, (float)x1, (float)y1, (float)x2, (float)y2);
}

void Renderer::drawRect(const SDL_FRect &rect)
{
    if (!initialized)
        return;

    SDL_RenderRect(renderer, &rect);
}

void Renderer::fillRect(const SDL_FRect &rect)
{
    if (!initialized)
        return;

    SDL_RenderFillRect(renderer, &rect);
}

void Renderer::getSize(int &width, int &height) const
{
    if (initialized && window)
    {
        SDL_GetWindowSize(window, &width, &height);
    }
    else
    {
        width = 0;
        height = 0;
    }
}

} // namespace tienlen
