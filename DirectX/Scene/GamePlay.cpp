#include "GamePlay.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Actor/Field.h"
#include "../Actor/PlayerActor.h"
#include "../Actor/Transform2D.h"
#include "../Camera/Camera.h"
#include "../Component/Collider.h"
#include "../Device/Physics.h"
#include "../Light/DirectionalLight.h"
#include "../Scene/Title.h"
#include "../Sprite/Sprite.h"
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

    auto kotori = new Sprite(mRenderer, "kotori.png");
    kotori->transform()->setScale(0.5f);
    auto size = kotori->transform()->getSize();
    kotori->transform()->setPivot(Vector2(-size.x / 2.f, size.y / 2.f));
    kotori->transform()->setPosition(Vector2(0.f, Game::WINDOW_HEIGHT));
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
