#include "FriedChickenManager.h"
#include "Actor.h"
#include "ActorCreater.h"
#include "Transform3D.h"
#include "../Device/Time.h"
#include "../Math/Math.h"

FriedChickenManager::FriedChickenManager() :
    mSpawnTimer(std::make_unique<Time>(2.f)),
    mMaxDrawNum(5) {
}

FriedChickenManager::~FriedChickenManager() = default;

void FriedChickenManager::initialize() {
    for (size_t i = 0; i < mMaxDrawNum; i++) {
        auto f = ActorCreater::create("FriedChicken");
        mChickens.emplace_back(f);
    }
}

std::shared_ptr<Actor> FriedChickenManager::FindNearestChicken(const ActorPtr target) {
    return FindNearestChicken(target, nullptr);
}

std::shared_ptr<Actor> FriedChickenManager::FindNearestChicken(const ActorPtr target, const ActorPtr exclude) {
    float nearest = Math::infinity;
    ActorPtr chicken = nullptr;
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
