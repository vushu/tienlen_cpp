#include "tienlen/menu_scene.hpp"
#include "tienlen/game_scene.hpp"
#include "tienlen/renderer.hpp"
#include <SDL3/SDL.h>

namespace tienlen
{

MenuScene::MenuScene()
    : selectedIndex(-1), transitionToGame(false)
{
}

bool MenuScene::init()
{
    // Create menu items centered on screen
    menuItems.clear();
    menuItems.emplace_back("Play Game", 220, 150, 200, 50);
    menuItems.emplace_back("Options", 220, 220, 200, 50);
    menuItems.emplace_back("Exit", 220, 290, 200, 50);

    return true;
}

void MenuScene::handleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        int mouseX = (int)event.motion.x;
        int mouseY = (int)event.motion.y;

        // Update hover state for all menu items
        for (size_t i = 0; i < menuItems.size(); ++i)
        {
            menuItems[i].hovered = checkMenuItemHover(menuItems[i], mouseX, mouseY);
            if (menuItems[i].hovered)
            {
                selectedIndex = (int)i;
            }
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT && selectedIndex >= 0)
        {
            SDL_Log("Menu item clicked: %s", menuItems[selectedIndex].text.c_str());
            
            // Handle menu selection
            if (selectedIndex == 0) // Play Game
            {
                transitionToGame = true;
            }
            else if (selectedIndex == 2) // Exit
            {
                SDL_Event quitEvent;
                quitEvent.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quitEvent);
            }
        }
    }
}

void MenuScene::update(double deltaTime)
{
    // Animation or other updates could go here
}

void MenuScene::render()
{
    auto &renderer = Renderer::getInstance();
    auto *sdlRenderer = renderer.getSDLRenderer();

    // Draw title
    SDL_SetRenderDrawColorFloat(sdlRenderer, 1.0f, 1.0f, 1.0f, 1.0f);
    SDL_FRect titleRect = {200, 50, 240, 60};
    SDL_RenderRect(sdlRenderer, &titleRect);

    // Draw menu items
    for (const auto &item : menuItems)
    {
        SDL_FRect rect = {item.x, item.y, item.width, item.height};

        // Set color based on hover state
        if (item.hovered)
        {
            SDL_SetRenderDrawColorFloat(sdlRenderer, 0.3f, 0.7f, 1.0f, 1.0f); // Light blue when hovered
        }
        else
        {
            SDL_SetRenderDrawColorFloat(sdlRenderer, 0.5f, 0.5f, 0.5f, 1.0f); // Gray normally
        }

        // Draw filled rectangle
        SDL_RenderFillRect(sdlRenderer, &rect);

        // Draw border
        SDL_SetRenderDrawColorFloat(sdlRenderer, 1.0f, 1.0f, 1.0f, 1.0f);
        SDL_RenderRect(sdlRenderer, &rect);
    }
}

void MenuScene::cleanup()
{
    menuItems.clear();
}

bool MenuScene::checkMenuItemHover(const MenuItem &item, int mouseX, int mouseY)
{
    return mouseX >= item.x && mouseX <= item.x + item.width &&
           mouseY >= item.y && mouseY <= item.y + item.height;
}

Scene *MenuScene::getNextScene()
{
    if (transitionToGame)
    {
        return new GameScene();
    }
    return nullptr;
}

} // namespace tienlen
