#pragma once

#include "SceneBase.h"
#include "../Utility/Input.h"
#include <memory>

enum class GameState {
    PLAY,
    PAUSED
};

class ActorManager;
class Physics;

class GamePlay : public SceneBase, public std::enable_shared_from_this<GamePlay> {
public:
    GamePlay();
    ~GamePlay();
    virtual void startScene() override;
    virtual void updateScene() override;

    GameState getState() const;
    void setState(GameState state);

private:
    ActorManager* mActorManager;
    Physics* mPhysics;
    GameState mState;
    KeyCode mPauseKey;
};
