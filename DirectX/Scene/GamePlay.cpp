#include "GamePlay.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Actor/Field.h"
#include "../Actor/PlayerActor.h"
#include "../Camera/Camera.h"
#include "../Component/Collider.h"
#include "../Device/Physics.h"
#include "../Scene/Title.h"
#include "../System/Game.h"
#include "../UI/Pause.h"

GamePlay::GamePlay() :
    SceneBase(),
    mActorManager(new ActorManager()),
    mPhysics(new Physics()),
    mState(GameState::PLAY) {
    Actor::setActorManager(mActorManager);
    Collider::setPhysics(mPhysics);
}

GamePlay::~GamePlay() {
    SAFE_DELETE(mActorManager);
    SAFE_DELETE(mPhysics);
    //なぜかColliderのPhysicsが生きてるから
    Collider::setPhysics(nullptr);
}

void GamePlay::startScene() {
    new PlayerActor(mRenderer);
    new Field(mRenderer);
    mCamera->setPlayer(mActorManager->getPlayer());
}

void GamePlay::updateScene() {
    if (mState == GameState::PLAY) {
        //総アクターアップデート
        mActorManager->update();
        //総当たり判定
        mPhysics->sweepAndPrune();
    } else if (mState == GameState::PAUSED) {

    }
}

GameState GamePlay::getState() const {
    return mState;
}

void GamePlay::setState(GameState state) {
    mState = state;
}
