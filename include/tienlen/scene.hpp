#pragma once

#include <SDL3/SDL.h>

namespace tienlen
{

    class Scene
    {
    public:
        virtual ~Scene() = default;

        // Initialize the scene
        virtual bool init() = 0;

        // Handle events
        virtual void handleEvent(const SDL_Event &event) = 0;

    // Update scene logic
    virtual void update(double deltaTime) = 0;

    // Render the scene
    virtual void render() = 0;

    // Cleanup the scene
    virtual void cleanup() = 0;        // Check if scene should transition to another scene
        virtual bool shouldTransition() const { return false; }

        // Get the next scene (if transitioning)
        virtual Scene *getNextScene() { return nullptr; }
    };

} // namespace tienlen
