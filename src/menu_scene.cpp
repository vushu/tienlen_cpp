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
    // Get actual window size
    auto &renderer = Renderer::getInstance();
    int windowWidth, windowHeight;
    SDL_GetRenderOutputSize(renderer.getSDLRenderer(), &windowWidth, &windowHeight);
    
    // Use larger buttons for better visibility on tablets
    float buttonWidth = windowWidth * 0.4f;  // 40% of screen width
    float buttonHeight = windowHeight * 0.12f; // 12% of screen height
    float spacing = buttonHeight * 0.3f; // Space between buttons
    
    // Calculate total height of all buttons plus spacing
    float totalHeight = (3 * buttonHeight) + (2 * spacing);
    
    // Center horizontally and vertically
    float baseX = (windowWidth - buttonWidth) / 2.0f;
    float startY = (windowHeight - totalHeight) / 2.0f;
    
    menuItems.clear();
    menuItems.emplace_back("Play Game", baseX, startY, buttonWidth, buttonHeight);
    menuItems.emplace_back("Options", baseX, startY + buttonHeight + spacing, buttonWidth, buttonHeight);
    menuItems.emplace_back("Exit", baseX, startY + 2 * (buttonHeight + spacing), buttonWidth, buttonHeight);

    SDL_Log("Window size: %dx%d", windowWidth, windowHeight);
    SDL_Log("Button size: %.0fx%.0f, spacing: %.0f, totalHeight: %.0f, startY: %.0f", 
            buttonWidth, buttonHeight, spacing, totalHeight, startY);
    SDL_Log("Menu items initialized:");
    for (size_t i = 0; i < menuItems.size(); ++i)
    {
        SDL_Log("  [%zu] %s: x=%f, y=%f, width=%f, height=%f",
                i, menuItems[i].text.c_str(),
                menuItems[i].x, menuItems[i].y, menuItems[i].width, menuItems[i].height);
    }

    // Scale font based on button height
    int fontSize = static_cast<int>(buttonHeight * 0.5f);
    
    // On Android, assets are accessed differently
#ifdef __ANDROID__
    const char* fontPath = "fonts/Roboto-Regular.ttf";
#else
    const char* fontPath = "assets/fonts/Roboto-Regular.ttf";
#endif
    
    if (!font.load(fontPath, fontSize))
    {
        SDL_Log("Warning: Could not load font from %s: %s", fontPath, SDL_GetError());
    }
    else
    {
        SDL_Log("Successfully loaded font from %s at size %d", fontPath, fontSize);
    }

    return true;
}

void MenuScene::handleEvent(const SDL_Event &event)
{
    int touchX = 0, touchY = 0;
    bool checkHover = false;
    bool checkClick = false;

    // Handle mouse events (desktop)
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        touchX = (int)event.motion.x;
        touchY = (int)event.motion.y;
        checkHover = true;
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            touchX = (int)event.button.x;
            touchY = (int)event.button.y;
            checkHover = true;
            checkClick = true;
        }
    }
    // Handle touch events (Android)
    else if (event.type == SDL_EVENT_FINGER_MOTION || event.type == SDL_EVENT_FINGER_DOWN)
    {
        // Use render output size to match button positioning coordinates
        auto &renderer = Renderer::getInstance();
        int width, height;
        SDL_GetRenderOutputSize(renderer.getSDLRenderer(), &width, &height);
        
        touchX = (int)(event.tfinger.x * width);
        touchY = (int)(event.tfinger.y * height);
        
        SDL_Log("Touch event: type=%d, normalized=(%.3f, %.3f), touchXY=(%d, %d), renderSize=%dx%d", 
                event.type, event.tfinger.x, event.tfinger.y, touchX, touchY, width, height);
        
        checkHover = true;
       
        if (event.type == SDL_EVENT_FINGER_DOWN)
        {
            checkClick = true;
        }
    }

    // Update hover state
    if (checkHover)
    {
        for (size_t i = 0; i < menuItems.size(); ++i)
        {
            bool wasHovered = menuItems[i].hovered;
            menuItems[i].hovered = checkMenuItemHover(menuItems[i], touchX, touchY);
            if (menuItems[i].hovered)
            {
                selectedIndex = (int)i;
                if (!wasHovered)
                {
                    SDL_Log("Item '%s' now hovered at (%d, %d)", menuItems[i].text.c_str(), touchX, touchY);
                }
            }
        }
    }

    // Handle click/tap
    if (checkClick)
    {
        SDL_Log("Click detected at (%d, %d), selectedIndex=%d", touchX, touchY, selectedIndex);
        
        if (selectedIndex >= 0)
        {
            SDL_Log("Menu item selected: %s", menuItems[selectedIndex].text.c_str());
            
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
    
    // Draw menu items with bright, visible colors
    for (const auto &item : menuItems)
    {
        SDL_FRect rect = {item.x, item.y, item.width, item.height};

        // Use bright colors for visibility
        if (item.hovered)
        {
            // Bright cyan when hovered
            SDL_SetRenderDrawColorFloat(sdlRenderer, 0.0f, 1.0f, 1.0f, 1.0f);
        }
        else
        {
            // Bright blue for normal state
            SDL_SetRenderDrawColorFloat(sdlRenderer, 0.0f, 0.5f, 1.0f, 1.0f);
        }

        // Draw filled rectangle for button background
        SDL_RenderFillRect(sdlRenderer, &rect);

        // Draw bright white border
        SDL_SetRenderDrawColorFloat(sdlRenderer, 1.0f, 1.0f, 1.0f, 1.0f);
        SDL_RenderRect(sdlRenderer, &rect);

        // Draw text centered in button
        if (font.isLoaded())
        {
            SDL_Color textColor = {255, 255, 255, 255}; // White text for good contrast
            int centerX = static_cast<int>(item.x + item.width / 2);
            int centerY = static_cast<int>(item.y + item.height / 2);
            font.drawTextCentered(item.text, centerX, centerY, textColor);
        }
    }
}

void MenuScene::cleanup()
{
    font.cleanup();
    menuItems.clear();
}

bool MenuScene::checkMenuItemHover(const MenuItem &item, int mouseX, int mouseY)
{
    bool isHovered = mouseX >= item.x && mouseX <= item.x + item.width &&
                     mouseY >= item.y && mouseY <= item.y + item.height;
    
    if (isHovered)
    {
        SDL_Log("Item '%s' HOVERED! Touch: (%d,%d), Bounds: (%f-%f x, %f-%f y)",
                item.text.c_str(), mouseX, mouseY,
                item.x, item.x + item.width, item.y, item.y + item.height);
    }
    
    return isHovered;
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
