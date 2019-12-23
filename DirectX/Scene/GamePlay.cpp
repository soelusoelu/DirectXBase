#include "GamePlay.h"
#include "../Actor/PlayerActor.h"
#include "../Component/Collider.h"
#include "../Device/Physics.h"
#include "../System/Game.h"

GamePlay::GamePlay(std::shared_ptr<Renderer> renderer) :
    SceneBase(renderer),
    mPhysics(new Physics()),
    mState(GameState::PLAY) {
    Collider::setPhysics(mPhysics);
    new PlayerActor(mRenderer);
}

GamePlay::~GamePlay() {
    SAFE_DELETE(mPhysics);
    //なぜかColliderのPhysicsが生きてるから
    Collider::setPhysics(nullptr);
}

void GamePlay::updateScene() {
    if (mState == GameState::PLAY) {
        //総当たり判定
        mPhysics->sweepAndPrune();
    }
}

GameState GamePlay::getState() const {
    return mState;
}

void GamePlay::setState(GameState state) {
    mState = state;
}
