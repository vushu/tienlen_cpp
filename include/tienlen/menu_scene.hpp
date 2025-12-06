#pragma once

#include "scene.hpp"
#include <string>
#include <vector>

namespace tienlen
{

struct MenuItem
{
    std::string text;
    float x, y;
    float width, height;
    bool hovered;

    MenuItem(const std::string &t, float posX, float posY, float w, float h)
        : text(t), x(posX), y(posY), width(w), height(h), hovered(false) {}
};

class MenuScene : public Scene
{
public:
    MenuScene();
    ~MenuScene() override = default;

    bool init() override;
    void handleEvent(const SDL_Event &event) override;
    void update(double deltaTime) override;
    void render() override;
    void cleanup() override;

    bool shouldTransition() const override { return transitionToGame; }
    Scene *getNextScene() override;

private:
    std::vector<MenuItem> menuItems;
    int selectedIndex;
    bool transitionToGame;
    bool checkMenuItemHover(const MenuItem &item, int mouseX, int mouseY);
};

} // namespace tienlen