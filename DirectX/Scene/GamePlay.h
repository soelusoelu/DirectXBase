#pragma once

#include "SceneBase.h"
#include <memory>

class FriedChickenManager;
class PlayerChickenConnection;

class GamePlay : public SceneBase {
    enum class State {
        PLAY,
        PAUSED
    };

public:
    GamePlay();
    ~GamePlay();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<FriedChickenManager> mFriedChickenManager;
    std::unique_ptr<PlayerChickenConnection> mPCConnection;
    State mState;
};
