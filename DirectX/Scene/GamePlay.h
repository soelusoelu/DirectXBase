#pragma once

#include "SceneBase.h"
#include "../Utility/Input.h"
#include <memory>

enum class GameState {
    PLAY,
    PAUSED
};

class ActorManager;
class DirectionalLight;
class SpotLight;
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
    std::shared_ptr<DirectionalLight> mDLight;
    std::shared_ptr<SpotLight> mSLight;
    Physics* mPhysics;
    GameState mState;
};
