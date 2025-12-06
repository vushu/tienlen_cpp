#pragma once

#include "scene.hpp"

namespace tienlen
{

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene() override = default;

    bool init() override;
    void handleEvent(const SDL_Event &event) override;
    void update(double deltaTime) override;
    void render() override;
    void cleanup() override;

private:
    double elapsedTime;
};

} // namespace tienlen
