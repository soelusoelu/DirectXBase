#include "GamePlay.h"
#include "ResultScene.h"
#include "../Component/ComponentManager.h"
#include "../Component/FriedChickenManager.h"
#include "../Component/JumpTarget.h"
#include "../Component/PlayerChickenConnection.h"
#include "../Component/Score.h"
#include "../Component/TimeLimit.h"
#include "../DebugLayer/DebugUtility.h"
#include "../DebugLayer/Inspector.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"
#include "../Scene/Title.h"
#include "../System/Game.h"

GamePlay::GamePlay() :
    SceneBase(),
    mFriedChickenManager(nullptr),
    mPCConnection(nullptr),
    mScore(nullptr),
    mTimeLimit(nullptr),
    mState(State::PLAY) {
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    //ファイルからアクターを読み込む
    auto p = GameObjectCreater::create("Player");
    auto fcm = GameObjectCreater::create("FriedChickenManager");
    mFriedChickenManager = fcm->componentManager()->getComponent<FriedChickenManager>();
    auto c = mFriedChickenManager->FindNearestChicken(p);
    auto pcc = GameObjectCreater::create("PlayerChickenConnection");
    mPCConnection = pcc->componentManager()->getComponent<PlayerChickenConnection>();
    mPCConnection->setPlayer(p);
    mPCConnection->setChicken(c);
    auto score = GameObjectCreater::createUI("Score");
    mScore = score->componentManager()->getComponent<Score>();
    auto tl = GameObjectCreater::createUI("TimeLimit");
    mTimeLimit = tl->componentManager()->getComponent<TimeLimit>();
    auto jt = GameObjectCreater::createUI("JumpTarget");
    auto f = GameObjectCreater::create("Field");

    DebugUtility::inspector()->setTarget(p);
}

void GamePlay::update() {
    if (mState == State::PLAY) {
        auto p = mGameObjectManager->getPlayer();
        //プレイヤーが乗ってる唐揚げを除く一番近い唐揚げを探す
        auto c = mFriedChickenManager->FindNearestChicken(p, mPCConnection->getChicken());
        mPCConnection->playerJumpTarget(c);

        if (mTimeLimit->isOverLimit()) {
            nextScene(std::make_shared<ResultScene>(mScore->getScore()));
        }
        if (Input::keyboard()->getKeyDown(KeyCode::Escape)) {
            Game::quit();
        }
    } else if (mState == State::PAUSE) {
    }
    //リセット
    if (Input::keyboard()->getKeyDown(KeyCode::R)) {
        nextScene(std::make_shared<GamePlay>());
    }
}
