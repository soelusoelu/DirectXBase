#include "GamePlay.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Actor/DirectionalLight.h"
#include "../Actor/Field.h"
#include "../Actor/PlayerActor.h"
#include "../Actor/PointLight.h"
#include "../Actor/SpotLight.h"
#include "../Camera/Camera.h"
#include "../Component/Collider.h"
#include "../Device/Physics.h"
#include "../Scene/Title.h"
#include "../System/Game.h"
#include "../UI/Pause.h"

GamePlay::GamePlay() :
    SceneBase(),
    mActorManager(new ActorManager()),
    mDLight(nullptr),
    mPLight(nullptr),
    mSLight(nullptr),
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
    mDLight = std::make_shared<DirectionalLight>(mRenderer);
    //mPLight = std::make_shared<PointLight>(mRenderer);
    //mSLight = std::make_shared<SpotLight>(mRenderer);
    mCamera->setPlayer(mActorManager->getPlayer());
}

void GamePlay::updateScene() {
    if (mState == GameState::PLAY) {
        //総アクターアップデート
        mActorManager->update();
        //総当たり判定
        mPhysics->sweepAndPrune();
        //ライト関連
        mDLight->update();
        //mPLight->update();
        //mSLight->update();
    } else if (mState == GameState::PAUSED) {

    }
}

GameState GamePlay::getState() const {
    return mState;
}

void GamePlay::setState(GameState state) {
    mState = state;
}
