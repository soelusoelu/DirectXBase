#include "FriedChickenManager.h"
#include "../Component/ComponentManager.h"
#include "../Component/FriedChickenComponent.h"
#include "../Component/Score.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/Transform3D.h"
#include "../Math/Math.h"

FriedChickenManager::FriedChickenManager() :
    mSpawnTimer(std::make_unique<Time>(2.f)),
    mMaxDrawNum(10),
    mScore(nullptr) {
}

FriedChickenManager::~FriedChickenManager() = default;

void FriedChickenManager::initialize() {
    for (size_t i = 0; i < mMaxDrawNum; i++) {
        auto f = GameObjectCreater::create("FriedChicken");
        mChickens.emplace_back(f);
    }
}

void FriedChickenManager::update() {
    for (const auto& chicken : mChickens) {
        auto c = chicken->componentManager()->getComponent<FriedChickenComponent>();
        if (!c->successFrying()) {
            continue;
        }
        if (mScore) {
            mScore->addScore(10);
            mWaitingChickens.emplace_back(chicken);
            chicken->setActive(false);
        }
    }
}

std::shared_ptr<GameObject> FriedChickenManager::FindNearestChicken(const GameObjectPtr target) {
    return FindNearestChicken(target, nullptr);
}

std::shared_ptr<GameObject> FriedChickenManager::FindNearestChicken(const GameObjectPtr target, const GameObjectPtr exclude) {
    float nearest = Math::infinity;
    GameObjectPtr chicken = nullptr;
    for (const auto& c : mChickens) {
        if (c == exclude) {
            continue;
        }
        auto l = (c->transform()->getPosition() - target->transform()->getPosition()).lengthSq();
        if (l < nearest) {
            nearest = l;
            chicken = c;
        }
    }
    return chicken;
}

void FriedChickenManager::setScore(const GameObjectPtr score) {
    mScore = score->componentManager()->getComponent<Score>();
}

void FriedChickenManager::moveToWait() {
    //auto itr = mChickens.begin();
    //while (itr != mChickens.end()) {
    //    if ((*itr)->isDead()) {
    //        itr = mChickens.erase(itr);
    //    } else {
    //        ++itr;
    //    }
    //}
}
