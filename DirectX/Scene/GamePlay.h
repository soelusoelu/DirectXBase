#pragma once

#include "SceneBase.h"
#include <memory>

enum class GameState {
    PLAY,
};

class Physics;
class Renderer;

class GamePlay : public SceneBase, public std::enable_shared_from_this<GamePlay> {
public:
    GamePlay(std::shared_ptr<Renderer> renderer);
    ~GamePlay();
    virtual void updateScene() override;

    GameState getState() const;
    void setState(GameState state);

private:
    Physics* mPhysics;
    GameState mState;
};
