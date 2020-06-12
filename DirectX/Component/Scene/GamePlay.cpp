#include "GamePlay.h"
#include "Scene.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/FriedChickenComponent.h"
#include "../../Component/FriedChickenManager.h"
#include "../../Component/JumpTarget.h"
#include "../../Component/Oil.h"
#include "../../Component/OneRemain.h"
#include "../../Component/PlayerChickenConnection.h"
#include "../../Component/Score.h"
#include "../../Component/SoundComponent.h"
#include "../../Component/TimeLimit.h"
#include "../../DebugLayer/DebugUtility.h"
#include "../../DebugLayer/Inspector.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../GameObject/Transform3D.h"
#include "../../Input/Input.h"

GamePlay::GamePlay() :
    Component(),
    mScene(nullptr),
    mFriedChickenManager(nullptr),
    mPCConnection(nullptr),
    mScore(nullptr),
    mTimeLimit(nullptr),
    mJumpTarget(nullptr),
    mOil(nullptr),
    mOneRemain(nullptr),
    mIsFirstSleep(false) {
}

GamePlay::~GamePlay() = default;

void GamePlay::awake() {
    auto p = GameObjectCreater::create("Player");
    auto fcm = GameObjectCreater::create("FriedChickenManager");
    mFriedChickenManager = fcm->componentManager()->getComponent<FriedChickenManager>();
    auto c = mFriedChickenManager->findNearestChicken(*p);
    auto pcc = GameObjectCreater::create("PlayerChickenConnection");
    mPCConnection = pcc->componentManager()->getComponent<PlayerChickenConnection>();
    mPCConnection->setPlayer(*p);
    mPCConnection->setChicken(c);
    GameObjectCreater::create("Bird");
    auto score = GameObjectCreater::create("Score");
    mScore = score->componentManager()->getComponent<Score>();
    auto tl = GameObjectCreater::create("TimeLimit");
    mTimeLimit = tl->componentManager()->getComponent<TimeLimit>();
    auto jt = GameObjectCreater::create("JumpTarget");
    mJumpTarget = jt->componentManager()->getComponent<JumpTarget>();
    GameObjectCreater::create("Field");
    auto oil = GameObjectCreater::create("Oil");
    mOil = oil->componentManager()->getComponent<Oil>();
    auto oneRe = GameObjectCreater::create("OneRemain");
    mOneRemain = oneRe->componentManager()->getComponent<OneRemain>();
    mOneRemain->addObserver(mPCConnection);

    DebugUtility::inspector()->setTarget(p);
}

void GamePlay::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();
    mScene->addObjectToNext("Score");

    auto sounds = owner()->componentManager()->getComponents<SoundComponent>();
    for (const auto& sound : sounds) {
        sound->playBGM();
    }
}

void GamePlay::update() {
    if (!mIsFirstSleep) {
        mIsFirstSleep = true;
        return;
    }
    const auto& p = owner()->getGameObjectManager()->find("Player");
    //プレイヤーが乗ってる唐揚げを除く一番近い唐揚げを探す
    auto c = mFriedChickenManager->findNearestChicken(*p, mPCConnection->getChicken());
    mPCConnection->setPlayerJumpTarget(c);
    //ジャンプ地点を更新する
    if (mPCConnection->isJumpTarget()) {
        mJumpTarget->setActive(true);
        mJumpTarget->setTargetPosition(mPCConnection->getJumpTargetTopPos());
    } else {
        mJumpTarget->setActive(false);
    }
    //警告が必要なときは表示する
    mOneRemain->setPosition(mPCConnection->getChicken()->owner()->transform()->getPosition());
    //油の流れに沿って移動させる
    mOil->flow(p);
    auto list = mFriedChickenManager->getFriedChickens();
    for (auto&& c : list) {
        mOil->flow(c);
    }
    //スコアを加算
    auto score = mFriedChickenManager->getEvaluatedScore();
    if (score > 0) {
        mScore->addScore(score);
    }

    //タイマーが0になったらシーン切り替え
    if (mTimeLimit->isTime()) {
        mScene->next("ResultScene");
    }

#ifdef _DEBUG
    //リセット
    if (Input::keyboard()->getKeyDown(KeyCode::R)) {
        mScene->next("GamePlay");
    }
#endif // _DEBUG
}
