#pragma once

#include "SceneBase.h"
#include "../Utility/Input.h"
#include <memory>

class ActorManager;
class Physics;

class GamePlay : public SceneBase, public std::enable_shared_from_this<GamePlay> {
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
