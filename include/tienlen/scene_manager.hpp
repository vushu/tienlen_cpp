#pragma once

#include "scene.hpp"
#include <memory>

namespace tienlen
{

class SceneManager
{
public:
    // Get singleton instance
    static SceneManager &getInstance();

    // Delete copy constructor and assignment operator
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

    // Set the current scene
    void setScene(std::unique_ptr<Scene> scene);

    // Update and render current scene
    void handleEvent(const SDL_Event &event);
    void update(double deltaTime);
    void render();

    // Get current scene
    Scene *getCurrentScene() const { return currentScene.get(); }

    // Check if there's an active scene
    bool hasScene() const { return currentScene != nullptr; }

private:
    SceneManager();
    ~SceneManager() = default;

    std::unique_ptr<Scene> currentScene;
};

} // namespace tienlen
