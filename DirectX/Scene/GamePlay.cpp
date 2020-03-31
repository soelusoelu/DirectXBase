#include "GamePlay.h"
#include "../Actor/FriedChickenManager.h"
#include "../Camera/Camera.h"
#include "../Component/Collider.h"
#include "../Component/JumpTarget.h"
#include "../Connect/ChickenScoreConnection.h"
#include "../Connect/PlayerChickenConnection.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"
#include "../Light/DirectionalLight.h"
#include "../Scene/Title.h"
#include "../System/Game.h"

GamePlay::GamePlay() :
    SceneBase(),
    mFriedChickenManager(std::make_unique<FriedChickenManager>()),
    mPCConnection(std::make_unique<PlayerChickenConnection>()),
    mCSConnection(std::make_unique<ChickenScoreConnection>()),
    mPhysics(new Physics()),
    mState(State::PLAY) {
    Collider::setPhysics(mPhysics);
}

GamePlay::~GamePlay() {
    SAFE_DELETE(mPhysics);
    //なぜかColliderのPhysicsが生きてるから
    Collider::setPhysics(nullptr);
}

void GamePlay::start() {
    //ファイルからアクターを読み込む
    auto p = GameObjectCreater::create("Player");
    auto f = GameObjectCreater::create("Field");
    mFriedChickenManager->initialize();
    auto c = mFriedChickenManager->FindNearestChicken(p);
    mPCConnection->setPlayer(p);
    mPCConnection->setChicken(c);
    mPCConnection->initialize();
    auto score = GameObjectCreater::createUI("Score");
    auto jt = GameObjectCreater::createUI("JumpTarget");
    mCSConnection->setChicken(c);
    mCSConnection->setScore(score);

    mRenderer->getDirectionalLight()->createMesh(mRenderer);
    Debug::inspector()->setTarget(p);
}

void GamePlay::update() {
    if (mState == State::PLAY) {
        auto p = mGameObjectManager->getPlayer();
        //プレイヤーが乗ってる唐揚げを除く一番近い唐揚げを探す
        auto c = mFriedChickenManager->FindNearestChicken(p, mPCConnection->getChicken());
        mPCConnection->playerJumpTarget(c);
        mCSConnection->setChicken(mPCConnection->getChicken());
        //連結クラス
        mPCConnection->connect();
        mCSConnection->connect();
        //総当たり判定
        mPhysics->sweepAndPrune();

        if (Input::keyboard()->getKeyDown(KeyCode::Z)) {
            nextScene(std::make_shared<Title>());
        }
        if (Input::keyboard()->getKeyDown(KeyCode::Escape)) {
            Game::quit();
        }
    } else if (mState == State::PAUSED) {

    }
}
