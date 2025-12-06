#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace tienlen
{

class Renderer
{
public:
    // Get singleton instance
    static Renderer &getInstance();

    // Delete copy constructor and assignment operator
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    // Initialize the renderer with window parameters
    bool init(const std::string &title, int width, int height, bool resizable = true);

    // Cleanup resources
    void cleanup();

    // Clear the screen with a color
    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    // Present the rendered frame
    void present();

    // Set draw color
    void setDrawColor(float r, float g, float b, float a = 1.0f);

    // Draw primitives
    void drawPoint(int x, int y);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawRect(const SDL_FRect &rect);
    void fillRect(const SDL_FRect &rect);

    // Get raw SDL objects (for advanced usage)
    SDL_Window *getWindow() const { return window; }
    SDL_Renderer *getSDLRenderer() const { return renderer; }

    // Window properties
    void getSize(int &width, int &height) const;
    bool isInitialized() const { return initialized; }

private:
    Renderer();
    ~Renderer();

    SDL_Window *window;
    SDL_Renderer *renderer;
    bool initialized;
};

} // namespace tienlen
