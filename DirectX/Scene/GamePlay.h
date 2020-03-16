#pragma once

#include "SceneBase.h"

class ActorManager;
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

    void setPlay();
    void setPause();

private:
    ActorManager* mActorManager;
    Physics* mPhysics;
    State mState;
};
