#include "GamePlay.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Actor/DirectionalLight.h"
#include "../Actor/Field.h"
#include "../Actor/PlayerActor.h"
#include "../Camera/Camera.h"
#include "../Component/Collider.h"
#include "../Device/Physics.h"
#include "../Scene/Title.h"
#include "../System/Game.h"

GamePlay::GamePlay() :
    SceneBase(),
    mActorManager(new ActorManager()),
    mDLight(nullptr),
    mPhysics(new Physics()),
    mState(State::PLAY) {
    Actor::setActorManager(mActorManager);
    Collider::setPhysics(mPhysics);
}

GamePlay::~GamePlay() {
    SAFE_DELETE(mActorManager);
    SAFE_DELETE(mPhysics);
    //なぜかColliderのPhysicsが生きてるから
    Collider::setPhysics(nullptr);
}

void GamePlay::start() {
    new PlayerActor(mRenderer);
    new Field(mRenderer);
    mDLight = std::make_shared<DirectionalLight>(mRenderer);
    mCamera->setPlayer(mActorManager->getPlayer());
}

void GamePlay::update() {
    if (mState == State::PLAY) {
        //総アクターアップデート
        mActorManager->update();
        //総当たり判定
        mPhysics->sweepAndPrune();
        //ライト関連
        mDLight->update();
    } else if (mState == State::PAUSED) {

    }
}

void GamePlay::setPlay() {
    mState = State::PLAY;
}

void GamePlay::setPause() {
    mState = State::PAUSED;
}
