#include "GamePlayComponent.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/FriedChickenComponent.h"
#include "../../Component/FriedChickenManager.h"
#include "../../Component/JumpTarget.h"
#include "../../Component/Oil.h"
#include "../../Component/PlayerChickenConnection.h"
#include "../../Component/Score.h"
#include "../../Component/Timer.h"
#include "../../DebugLayer/DebugUtility.h"
#include "../../DebugLayer/Inspector.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"
#include "../../Utility/LevelLoader.h"

GamePlayComponent::GamePlayComponent(const std::shared_ptr<GameObject>& owner) :
    Component(owner, "GamePlayComponent"),
    mFriedChickenManager(nullptr),
    mPCConnection(nullptr),
    mScore(nullptr),
    mTimeLimitTimer(nullptr),
    mJumpTarget(nullptr),
    mOil(nullptr),
    mIsEnd(false),
    mIsFirstSleep(false) {
}

GamePlayComponent::~GamePlayComponent() = default;

void GamePlayComponent::awake() {
    auto p = GameObjectCreater::create("Player");
    auto fcm = GameObjectCreater::create("FriedChickenManager");
    mFriedChickenManager = fcm->componentManager()->getComponent<FriedChickenManager>();
    auto c = mFriedChickenManager->findNearestChicken(*p);
    auto pcc = GameObjectCreater::create("PlayerChickenConnection");
    mPCConnection = pcc->componentManager()->getComponent<PlayerChickenConnection>();
    mPCConnection->setPlayer(*p);
    mPCConnection->setChicken(c);
    GameObjectCreater::create("Bird");
    auto score = GameObjectCreater::createUI("Score");
    mScore = score->componentManager()->getComponent<Score>();
    auto tl = GameObjectCreater::createUI("TimeLimit");
    mTimeLimitTimer = tl->componentManager()->getComponent<Timer>();
    auto jt = GameObjectCreater::createUI("JumpTarget");
    mJumpTarget = jt->componentManager()->getComponent<JumpTarget>();
    //GameObjectCreater::create("Field");
    auto oil = GameObjectCreater::create("Oil");
    mOil = oil->componentManager()->getComponent<Oil>();

    DebugUtility::inspector()->setTarget(p);
}

void GamePlayComponent::update() {
    if (!mIsFirstSleep) {
        mIsFirstSleep = true;
        return;
    }
    auto p = owner()->getGameObjectManager()->getPlayer();
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
    //油の流れに沿って移動させる
    mOil->flow(p);
    auto list = mFriedChickenManager->getFriedChickens();
    for (auto&& c : list) {
        mOil->flow(c);
    }
    //スコアを加算
    auto score = mFriedChickenManager->getEvaluatedScore();
    mScore->addScore(score);

    mIsEnd = mTimeLimitTimer->isTime();
}

bool GamePlayComponent::isEnd() const {
    return mIsEnd;
}
