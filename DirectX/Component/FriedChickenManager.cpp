﻿#include "FriedChickenManager.h"
#include "ChickenFry.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "Score.h"
#include "ScoreEvaluation.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

FriedChickenManager::FriedChickenManager(std::shared_ptr<GameObject> owner) :
    Component(owner, "FriedChickenManager", 200),
    mScore(nullptr),
    mScoreEvaluation(nullptr),
    mMaxDrawNum(10) {
}

FriedChickenManager::~FriedChickenManager() = default;

void FriedChickenManager::awake() {
    for (size_t i = 0; i < mMaxDrawNum; i++) {
        auto f = GameObjectCreater::create("FriedChicken");
        auto c = f->componentManager()->getComponent<FriedChickenComponent>();
        mChickens.emplace_back(c);
    }
}

void FriedChickenManager::start() {
    auto score = owner()->getGameObjectManager()->find("Score");
    mScore = score->componentManager()->getComponent<Score>();
    mScoreEvaluation = owner()->componentManager()->getComponent<ScoreEvaluation>();

    for (auto&& chicken : mChickens) {
        chicken->firstSet(mScoreEvaluation->getGood());
    }
}

void FriedChickenManager::update() {
    addScore();
    moveToWait();
    replenish();
}

void FriedChickenManager::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getInt(inObj, "maxDrawNum", &mMaxDrawNum);
}

void FriedChickenManager::drawDebugInfo(DebugInfoList * inspect) const {
    DebugInfo info;
    info.first = "MaxDrawNum";
    info.second = StringUtil::intToString(mMaxDrawNum);
    inspect->emplace_back(info);
}

std::shared_ptr<FriedChickenComponent> FriedChickenManager::findNearestChicken(const GameObjectPtr & target) const {
    return findNearestChicken(target, nullptr);
}

std::shared_ptr<FriedChickenComponent> FriedChickenManager::findNearestChicken(const GameObjectPtr & target, const ChickenPtr & exclude) const {
    float nearest = Math::infinity;
    ChickenPtr chicken = nullptr;
    for (const auto& c : mChickens) {
        if (c == exclude) {
            continue;
        }
        if (!c->isFrying()) {
            continue;
        }
        auto l = (c->owner()->transform()->getPosition() - target->transform()->getPosition()).lengthSq();
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

void FriedChickenManager::addScore() {
    if (!mScoreEvaluation) {
        return;
    }

    for (const auto& chicken : mChickens) {
        if (!chicken->isFinished()) {
            continue;
        }
        auto fry = chicken->owner()->componentManager()->getComponent<ChickenFry>();
        int score = 0;
        if (!fry) {
            return;
        }
        score = mScoreEvaluation->evaluateScore(*fry);
        if (mScore) {
            mScore->addScore(score);
        }
    }
}

void FriedChickenManager::moveToWait() {
    auto itr = mChickens.begin();
    while (itr != mChickens.end()) {
        if ((*itr)->isFinished() || (*itr)->isEaten()) {
            mWaitingChickens.emplace_back(*itr);
            (*itr)->owner()->setActive(false);
            itr = mChickens.erase(itr);
        } else {
            ++itr;
        }
    }
}

void FriedChickenManager::replenish() {
    if (mChickens.size() < mMaxDrawNum) {
        if (mWaitingChickens.size() == 0) {
            return;
        }

        auto itr = mWaitingChickens.begin();
        auto chicken = (*itr);
        mWaitingChickens.erase(itr);
        chicken->initialize();
        mChickens.emplace_back(chicken);
    }
}
