#include "tienlen/scene_manager.hpp"
#include <SDL3/SDL.h>

namespace tienlen
{

SceneManager &SceneManager::getInstance()
{
    static SceneManager instance;
    return instance;
}

SceneManager::SceneManager()
    : currentScene(nullptr)
{
}

void SceneManager::setScene(std::unique_ptr<Scene> scene)
{
    // Cleanup old scene
    if (currentScene)
    {
        currentScene->cleanup();
    }

    // Set new scene
    currentScene = std::move(scene);

    // Initialize new scene
    if (currentScene)
    {
        if (!currentScene->init())
        {
            SDL_Log("Failed to initialize scene");
            currentScene.reset();
        }
    }
}

void SceneManager::handleEvent(const SDL_Event &event)
{
    if (currentScene)
    {
        currentScene->handleEvent(event);

        // Check for scene transition
        if (currentScene->shouldTransition())
        {
            Scene *nextScene = currentScene->getNextScene();
            if (nextScene)
            {
                setScene(std::unique_ptr<Scene>(nextScene));
            }
        }
    }
}

void SceneManager::update(double deltaTime)
{
    if (currentScene)
    {
        currentScene->update(deltaTime);
    }
}

void SceneManager::render()
{
    if (currentScene)
    {
        currentScene->render();
    }
}

} // namespace tienlen
