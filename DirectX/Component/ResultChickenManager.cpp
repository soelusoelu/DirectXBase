#include "ResultChickenManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Utility/LevelLoader.h"

ResultChickenManager::ResultChickenManager() :
    Component(),
    mFallsNum(0) {
}

ResultChickenManager::~ResultChickenManager() = default;

void ResultChickenManager::awake() {
    for (size_t i = 0; i < mFallsNum; i++) {
        auto gameObject = GameObjectCreater::create("ResultChickenFall");
        mChickens.emplace_back(gameObject);
    }
}

void ResultChickenManager::finalize() {
    for (auto&& c : mChickens) {
        c->destroy();
    }
}

void ResultChickenManager::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "fallsNum", &mFallsNum);
}

void ResultChickenManager::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FallsNum", mFallsNum);
}
