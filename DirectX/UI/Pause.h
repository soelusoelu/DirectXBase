#pragma once

#include "UI.h"
#include "../Utility/Input.h"
#include <memory>

class GamePlay;

class Pause : public UI {
public:
    Pause(std::shared_ptr<GamePlay> game);
    ~Pause();
    virtual void update() override;

private:
    std::weak_ptr<GamePlay> mGame;
    KeyCode mCloseKey;
};
