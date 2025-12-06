#include "tienlen/font.hpp"
#include "tienlen/renderer.hpp"
#include <SDL3/SDL.h>

namespace tienlen
{

Font::Font()
    : font(nullptr), fontSize(0)
{
}

Font::~Font()
{
    cleanup();
}

bool Font::load(const std::string &path, int pointSize)
{
    cleanup();

    font = TTF_OpenFont(path.c_str(), pointSize);
    if (!font)
    {
        SDL_Log("Failed to load font %s: %s", path.c_str(), SDL_GetError());
        return false;
    }

    fontSize = pointSize;
    return true;
}

void Font::cleanup()
{
    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
        fontSize = 0;
    }
}

void Font::drawText(const std::string &text, int x, int y, SDL_Color color)
{
    if (!font || text.empty())
        return;

    auto &renderer = Renderer::getInstance();
    auto *sdlRenderer = renderer.getSDLRenderer();

    // Render text to surface
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
    if (!surface)
    {
        SDL_Log("Failed to render text: %s", SDL_GetError());
        return;
    }

    // Create texture from surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
    if (!texture)
    {
        SDL_Log("Failed to create texture from text: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }

    // Get texture dimensions
    int texW = surface->w;
    int texH = surface->h;
    SDL_DestroySurface(surface);

    // Render texture
    SDL_FRect destRect = {(float)x, (float)y, (float)texW, (float)texH};
    SDL_RenderTexture(sdlRenderer, texture, NULL, &destRect);

    // Cleanup
    SDL_DestroyTexture(texture);
}

void Font::drawTextCentered(const std::string &text, int centerX, int centerY, SDL_Color color)
{
    int width, height;
    getTextSize(text, width, height);
    drawText(text, centerX - width / 2, centerY - height / 2, color);
}

void Font::getTextSize(const std::string &text, int &width, int &height)
{
    width = 0;
    height = 0;

    if (!font || text.empty())
        return;

    if (!TTF_GetStringSize(font, text.c_str(), text.length(), &width, &height))
    {
        SDL_Log("Failed to get text size: %s", SDL_GetError());
    }
}

} // namespace tienlen
