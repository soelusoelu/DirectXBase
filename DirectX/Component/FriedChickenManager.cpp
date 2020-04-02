﻿#include "FriedChickenManager.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "Score.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/Transform3D.h"
#include "../Math/Math.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

FriedChickenManager::FriedChickenManager(std::shared_ptr<GameObject> owner) :
    Component(owner, "FriedChickenManager", 200),
    mSpawnTimer(std::make_unique<Time>(2.f)),
    mMaxDrawNum(10),
    mScore(nullptr) {
}

FriedChickenManager::~FriedChickenManager() = default;

void FriedChickenManager::awake() {
    for (size_t i = 0; i < mMaxDrawNum; i++) {
        auto f = GameObjectCreater::create("FriedChicken");
        auto c = f->componentManager()->getComponent<FriedChickenComponent>();
        mChickens.emplace_back(c);
    }
    auto f = GameObjectCreater::create("FriedChicken");
    owner()->transform()->addChild(f->transform());
    f->destroy(2.f);
}

void FriedChickenManager::update() {
    for (const auto& chicken : mChickens) {
        if (!chicken->successFrying()) {
            continue;
        }
        if (mScore) {
            mScore->addScore(10);
        }
    }
    moveToWait();
}

void FriedChickenManager::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getInt(inObj, "maxDrawNum", &mMaxDrawNum);
}

void FriedChickenManager::drawDebugInfo(debugInfoList* inspect) const {
    debugInfo info;
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
        auto l = (c->owner()->transform()->getPosition() - target->transform()->getPosition()).lengthSq();
        if (l < nearest) {
            nearest = l;
            chicken = c->owner();
        }
    }
    return chicken;
}

void FriedChickenManager::setScore(const GameObjectPtr score) {
    mScore = score->componentManager()->getComponent<Score>();
}

void FriedChickenManager::moveToWait() {
    auto itr = mChickens.begin();
    while (itr != mChickens.end()) {
        if ((*itr)->successFrying()) {
            mWaitingChickens.emplace_back(*itr);
            (*itr)->owner()->setActive(false);
            itr = mChickens.erase(itr);
        } else {
            ++itr;
        }
    }
}