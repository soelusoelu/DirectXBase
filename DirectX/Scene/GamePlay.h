#pragma once

#include "SceneBase.h"
#include <memory>

class ActorManager;
class FriedChickenManager;
class PlayerChickenConnection;
class ChickenScoreConnection;
class Physics;

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
    ActorManager* mActorManager;
    std::unique_ptr<FriedChickenManager> mFriedChickenManager;
    std::unique_ptr<PlayerChickenConnection> mPCConnection;
    std::unique_ptr<ChickenScoreConnection> mCSConnection;
    Physics* mPhysics;
    State mState;
};
