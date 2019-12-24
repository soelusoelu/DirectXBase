#pragma once

#include "UI.h"
#include "../Utility/Input.h"
#include <memory>

class GamePlay;
class Renderer;

class Pause : public UI {
public:
    Pause(std::shared_ptr<GamePlay> game, std::shared_ptr<Renderer> renderer);
    ~Pause();
    virtual void updateUI() override;

private:
    std::weak_ptr<GamePlay> mGame;
    KeyCode mCloseKey;
};
