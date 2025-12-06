#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "tienlen/renderer.hpp"
#include "tienlen/scene_manager.hpp"
#include "tienlen/menu_scene.hpp"
#include <memory>

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Tien Len", "1.0", "com.bamhaven.tienlen");

    if (!tienlen::Renderer::getInstance().init("Tien Len", 640, 480, true))
    {
        SDL_Log("Couldn't initialize renderer");
        return SDL_APP_FAILURE;
    }

    // Start with menu scene
    tienlen::SceneManager::getInstance().setScene(std::make_unique<tienlen::MenuScene>());

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    }

    tienlen::SceneManager::getInstance().handleEvent(*event);

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    static Uint64 lastTicks = 0;
    Uint64 currentTicks = SDL_GetTicks();
    double deltaTime = lastTicks == 0 ? 0.0 : (currentTicks - lastTicks) / 1000.0;
    lastTicks = currentTicks;

    // Update current scene
    tienlen::SceneManager::getInstance().update(deltaTime);

    // Clear screen
    tienlen::Renderer::getInstance().clear(0.0f, 0.0f, 0.0f, 1.0f);

    // Render current scene
    tienlen::SceneManager::getInstance().render();

    // Present
    tienlen::Renderer::getInstance().present();

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    tienlen::Renderer::getInstance().cleanup();
}
