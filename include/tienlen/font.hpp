#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <memory>

namespace tienlen
{

class Font
{
public:
    Font();
    ~Font();

    // Load a font from file
    bool load(const std::string &path, int pointSize);

    // Cleanup resources
    void cleanup();

    // Draw text at specified position
    void drawText(const std::string &text, int x, int y, SDL_Color color = {255, 255, 255, 255});

    // Draw text centered at position
    void drawTextCentered(const std::string &text, int centerX, int centerY, SDL_Color color = {255, 255, 255, 255});

    // Get text dimensions
    void getTextSize(const std::string &text, int &width, int &height);

    // Check if font is loaded
    bool isLoaded() const { return font != nullptr; }

private:
    TTF_Font *font;
    int fontSize;
};

} // namespace tienlen
