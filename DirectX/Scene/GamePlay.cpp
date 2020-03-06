#include "GamePlay.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Camera/Camera.h"
#include "../Component/Collider.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../Light/DirectionalLight.h"
#include "../Scene/Title.h"
#include "../System/Game.h"
#include "../Utility/Input.h"
#include "../Utility/LevelLoader.h"

GamePlay::GamePlay() :
    SceneBase(),
    mActorManager(new ActorManager()),
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
    //ファイルからアクターを読み込む
    Singleton<LevelLoader>::instance().loadActors(mRenderer, "Actors.json");
    Singleton<LevelLoader>::instance().loadSpecifiedUI(mRenderer, "UIList.json", "Score");

    mRenderer->getDirectionalLight()->createMesh(mRenderer);

    mCamera->setPlayer(mActorManager->getPlayer());
}

void GamePlay::update() {
    if (mState == State::PLAY) {
        //総アクターアップデート
        mActorManager->update();
        //総当たり判定
        mPhysics->sweepAndPrune();

        if (Input::getKeyDown(KeyCode::Space)) {
            nextScene(std::make_shared<Title>());
        }
    } else if (mState == State::PAUSED) {

    }
}

void GamePlay::setPlay() {
    mState = State::PLAY;
}

void GamePlay::setPause() {
    mState = State::PAUSED;
}
