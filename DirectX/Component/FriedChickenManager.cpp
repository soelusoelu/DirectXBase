﻿#include "FriedChickenManager.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "ScoreEvaluation.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/Transform3D.h"
#include "../Math/Math.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

FriedChickenManager::FriedChickenManager(std::shared_ptr<GameObject> owner) :
    Component(owner, "FriedChickenManager", 200),
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

std::shared_ptr<GameObject> FriedChickenManager::FindNearestChicken(const GameObjectPtr target) {
    return FindNearestChicken(target, nullptr);
}

std::shared_ptr<GameObject> FriedChickenManager::FindNearestChicken(const GameObjectPtr target, const GameObjectPtr exclude) {
    float nearest = Math::infinity;
    GameObjectPtr chicken = nullptr;
    for (const auto& c : mChickens) {
        if (c->owner() == exclude) {
            continue;
        }
        if (!c->isFrying()) {
            continue;
        }
        auto l = (c->owner()->transform()->getPosition() - target->transform()->getPosition()).lengthSq();
        if (l < nearest) {
            nearest = l;
            chicken = c->owner();
        }
    }
    return chicken;
}

void FriedChickenManager::addScore() {
    if (!mScoreEvaluation) {
        return;
    }

    for (const auto& chicken : mChickens) {
        if (!chicken->isFinished()) {
            continue;
        }
        mScoreEvaluation->evaluateScore(*chicken);
    }
}

void FriedChickenManager::moveToWait() {
    auto itr = mChickens.begin();
    while (itr != mChickens.end()) {
        if ((*itr)->isFinished()) {
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
