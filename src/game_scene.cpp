#include "tienlen/game_scene.hpp"
#include "tienlen/renderer.hpp"
#include <SDL3/SDL.h>

namespace tienlen
{

GameScene::GameScene()
    : elapsedTime(0.0)
{
}

bool GameScene::init()
{
    elapsedTime = 0.0;
    SDL_Log("GameScene initialized");
    return true;
}

void GameScene::handleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.key == SDLK_ESCAPE)
        {
            SDL_Log("Escape pressed in game");
        }
    }
}

void GameScene::update(double deltaTime)
{
    elapsedTime += deltaTime;
}

void GameScene::render()
{
    auto &renderer = Renderer::getInstance();
    auto *sdlRenderer = renderer.getSDLRenderer();

    // Draw game background
    SDL_SetRenderDrawColorFloat(sdlRenderer, 0.1f, 0.3f, 0.2f, 1.0f);
    SDL_FRect bgRect = {0, 0, 640, 480};
    SDL_RenderFillRect(sdlRenderer, &bgRect);

    // Draw a simple playing area
    SDL_SetRenderDrawColorFloat(sdlRenderer, 0.2f, 0.6f, 0.3f, 1.0f);
    SDL_FRect tableRect = {50, 50, 540, 380};
    SDL_RenderFillRect(sdlRenderer, &tableRect);

    // Draw border
    SDL_SetRenderDrawColorFloat(sdlRenderer, 1.0f, 1.0f, 1.0f, 1.0f);
    SDL_RenderRect(sdlRenderer, &tableRect);
}

void GameScene::cleanup()
{
    SDL_Log("GameScene cleanup");
}

} // namespace tienlen
