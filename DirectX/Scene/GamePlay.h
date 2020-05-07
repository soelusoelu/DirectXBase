#pragma once

#include "SceneBase.h"
#include <memory>

class GamePlayComponent;

class GamePlay : public SceneBase {
    enum class State {
        PLAY,
        PAUSE,
    };

public:
    GamePlay();
    ~GamePlay();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<GamePlayComponent> mGamePlay;
    State mState;
};
