#include "GamePlay.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorCreater.h"
#include "../Actor/ActorManager.h"
#include "../Actor/Field.h"
#include "../Actor/FriedChickenManager.h"
#include "../Actor/PlayerActor.h"
#include "../Actor/Transform3D.h"
#include "../Camera/Camera.h"
#include "../Component/Collider.h"
#include "../Connect/ChickenScoreConnection.h"
#include "../Connect/PlayerChickenConnection.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"
#include "../Light/DirectionalLight.h"
#include "../Scene/Title.h"
#include "../System/Game.h"
#include "../UI/JumpTarget.h"
#include "../UI/Score.h"
#include "../UI/UICreater.h"

GamePlay::GamePlay() :
    SceneBase(),
    mActorManager(new ActorManager()),
    mFriedChickenManager(std::make_unique<FriedChickenManager>()),
    mPCConnection(std::make_unique<PlayerChickenConnection>()),
    mCSConnection(std::make_unique<ChickenScoreConnection>()),
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
    auto p = ActorCreater::create<PlayerActor>("Player");
    //auto f = ActorCreater::create<Field>("Field");
    mFriedChickenManager->initialize();
    auto c = mFriedChickenManager->FindNearestChicken(p);
    mPCConnection->setPlayer(p);
    mPCConnection->setChicken(c);
    mPCConnection->initialize();
    auto score = UICreater::create<Score>("Score");
    auto jt = UICreater::create<JumpTarget>("JumpTarget");
    mCSConnection->setChicken(c);
    mCSConnection->setScore(score);

    mRenderer->getDirectionalLight()->createMesh(mRenderer);
    Debug::inspector()->setTarget(p);
}

void GamePlay::update() {
    if (mState == State::PLAY) {
        auto p = mActorManager->getPlayer();
        //プレイヤーが乗ってる唐揚げを除く一番近い唐揚げを探す
        auto c = mFriedChickenManager->FindNearestChicken(p, mPCConnection->getChicken());
        mPCConnection->playerJumpTarget(c);
        mCSConnection->setChicken(mPCConnection->getChicken());
        //総アクターアップデート
        mActorManager->update();
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
