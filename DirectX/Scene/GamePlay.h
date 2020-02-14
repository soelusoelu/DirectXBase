#pragma once

#include "SceneBase.h"
#include "../Utility/Input.h"
#include <memory>

class ActorManager;
class DirectionalLight;
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
    std::shared_ptr<DirectionalLight> mDLight;
    Physics* mPhysics;
    State mState;
};
