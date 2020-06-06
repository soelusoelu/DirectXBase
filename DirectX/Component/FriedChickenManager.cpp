#include "FriedChickenManager.h"
#include "ChickenFry.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "ScoreEvaluation.h"
#include "SoundComponent.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

FriedChickenManager::FriedChickenManager() :
    Component(),
    mScoreEvaluation(nullptr),
    mSound(nullptr),
    mGoodFriedSound(nullptr),
    mStartNum(0),
    mMaxNum(0),
    mCurrentMaxNum(0),
    mReplenishTimer(std::make_unique<Time>()),
    mFriedNum(0) {
}

FriedChickenManager::~FriedChickenManager() = default;

void FriedChickenManager::awake() {
    for (size_t i = 0; i < mStartNum; i++) {
        auto f = GameObjectCreater::create("FriedChicken");
        auto c = f->componentManager()->getComponent<FriedChickenComponent>();
        mChickens.emplace_back(c);
    }
    int waitNum = mMaxNum - mStartNum;
    for (size_t i = 0; i < waitNum; i++) {
        auto f = GameObjectCreater::create("FriedChicken");
        auto c = f->componentManager()->getComponent<FriedChickenComponent>();
        mWaitingChickens.emplace_back(c);
        f->setActive(false);
    }

    mCurrentMaxNum = mStartNum;
}

void FriedChickenManager::start() {
    const auto& compMana = owner()->componentManager();
    mScoreEvaluation = compMana->getComponent<ScoreEvaluation>();
    auto sounds = compMana->getComponents<SoundComponent>();
    mSound = sounds[0];
    mGoodFriedSound = sounds[1];
}

void FriedChickenManager::update() {
    deactivateWaitingChickens();
    increaseTheMaximumNumber();
    moveToWait();
    replenish();
    goodFriedSound();
}

void FriedChickenManager::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getInt(inObj, "startNum", &mStartNum);
    JsonHelper::getInt(inObj, "maxNum", &mMaxNum);
    float time;
    if (JsonHelper::getFloat(inObj, "replenishTimer", &time)) {
        mReplenishTimer->setLimitTime(time);
    }
}

void FriedChickenManager::drawDebugInfo(ComponentDebug::DebugInfoList * inspect) const {
    inspect->emplace_back("StartNum", mStartNum);
    inspect->emplace_back("MaxNum", mMaxNum);
    inspect->emplace_back("CurrentMaxNum", mCurrentMaxNum);
    inspect->emplace_back("FriedNum", mFriedNum);
}

std::shared_ptr<FriedChickenComponent> FriedChickenManager::findNearestChicken(const GameObject & target) const {
    return findNearestChicken(target, nullptr);
}

std::shared_ptr<FriedChickenComponent> FriedChickenManager::findNearestChicken(const GameObject & target, const ChickenPtr & exclude) const {
    float nearest = Math::infinity;
    ChickenPtr chicken = nullptr;
    for (const auto& c : mChickens) {
        if (c == exclude) {
            continue;
        }
        if (!c->isFrying()) {
            continue;
        }
        if (!c->owner()->getActive()) {
            continue;
        }
        auto l = (c->owner()->transform()->getPosition() - target.transform()->getPosition()).lengthSq();
        if (l < nearest) {
            nearest = l;
            chicken = c;
        }
    }
    return chicken;
}

std::list<std::shared_ptr<GameObject>> FriedChickenManager::getFriedChickens() const {
    GameObjectPtrList list;
    for (const auto& c : mChickens) {
        if (!c->isFrying()) {
            continue;
        }
        list.emplace_back(c->owner());
    }
    return list;
}

int FriedChickenManager::getEvaluatedScore() const {
    int score = 0;
    for (const auto& chicken : mChickens) {
        if (!chicken->isWaitingScore()) {
            continue;
        }
        const auto& fry = chicken->getFry();
        score += mScoreEvaluation->evaluateScore(fry);

        if (chicken->isRise()) { //上昇状態だったらスコアの加算状態を切る
            chicken->setWaitingScore(false);
        } else { //上昇していなかったら即回収
            chicken->finishFryed();
        }
    }

    if (score != 0) {
        mSound->playSE();
    }

    return score;
}

void FriedChickenManager::deactivateWaitingChickens() {
    for (auto&& c : mWaitingChickens) {
        if (c->owner()->getActive()) {
            c->owner()->setActive(false);
        }
    }
}

void FriedChickenManager::increaseTheMaximumNumber() {
    mReplenishTimer->update();
    if (mReplenishTimer->isTime()) {
        mReplenishTimer->reset();
        mCurrentMaxNum++;
    }

    if (mCurrentMaxNum > mMaxNum) {
        mCurrentMaxNum = mMaxNum;
    }
}

void FriedChickenManager::moveToWait() {
    auto itr = mChickens.begin();
    while (itr != mChickens.end()) {
        if ((*itr)->isFinished()) {
            mWaitingChickens.emplace_back(*itr);
            (*itr)->owner()->setActive(false);
            itr = mChickens.erase(itr);

            mFriedNum++;
        } else {
            ++itr;
        }
    }
}

void FriedChickenManager::replenish() {
    if (mChickens.size() >= mCurrentMaxNum) {
        return;
    }
    if (mWaitingChickens.empty()) {
        return;
    }

    auto itr = mWaitingChickens.begin();
    auto chicken = (*itr);
    mWaitingChickens.erase(itr);
    chicken->initialize();
    mChickens.emplace_back(chicken);
}

void FriedChickenManager::goodFriedSound() {
    static bool isPlay = false;
    static float timer = 0.f;
    if (isPlay) {
        return;
    }
    if (mFriedNum < 3) {
        return;
    }
    timer += Time::deltaTime;
    if (timer > 10.f) {
        mGoodFriedSound->playSE();
        isPlay = true;
    }
}
