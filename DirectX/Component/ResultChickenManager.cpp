#include "ResultChickenManager.h"
#include "../DebugLayer/Debug.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Utility/LevelLoader.h"

ResultChickenManager::ResultChickenManager() :
    Component(),
    mLowNum(0),
    mMiddleNum(0),
    mHighNum(0),
    mSuperNum(0) {
}

ResultChickenManager::~ResultChickenManager() = default;

void ResultChickenManager::finalize() {
    for (auto&& c : mChickens) {
        c->destroy();
    }
}

void ResultChickenManager::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "lowNum", &mLowNum);
    JsonHelper::getInt(inObj, "middleNum", &mMiddleNum);
    JsonHelper::getInt(inObj, "highNum", &mHighNum);
    JsonHelper::getInt(inObj, "superNum", &mSuperNum);
}

void ResultChickenManager::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("LowNum", mLowNum);
    inspect->emplace_back("MiddleNum", mMiddleNum);
    inspect->emplace_back("HighNum", mHighNum);
    inspect->emplace_back("SuperNum", mSuperNum);
}

void ResultChickenManager::createChickensFromRank(Rank rank) {
    int num = 0;
    if (rank == Rank::LOW) {
        num = mLowNum;
    } else if (rank == Rank::MIDDLE) {
        num = mMiddleNum;
    } else if (rank == Rank::HIGH) {
        num = mHighNum;
    } else if (rank == Rank::SUPER) {
        num = mSuperNum;
    } else {
        Debug::logWarning(getComponentName() + ": Rank is None.");
    }

    for (size_t i = 0; i < num; i++) {
        auto gameObject = GameObjectCreater::create("ResultChickenFall");
        mChickens.emplace_back(gameObject);
    }
}
