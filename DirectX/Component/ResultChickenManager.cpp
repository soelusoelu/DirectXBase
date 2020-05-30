#include "ResultChickenManager.h"
#include "ComponentManager.h"
#include "ResultChickenFall.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Utility/LevelLoader.h"

ResultChickenManager::ResultChickenManager() :
    Component(),
    mFallsNum(0)
{
}

ResultChickenManager::~ResultChickenManager() = default;

void ResultChickenManager::awake() {
    for (size_t i = 0; i < mFallsNum; i++) {
        auto gameObject = GameObjectCreater::create("ResultChickenFall");
        auto rcf = gameObject->componentManager()->getComponent<ResultChickenFall>();
        mChickens.emplace_back(rcf);
    }
}

void ResultChickenManager::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "fallsNum", &mFallsNum);
}

void ResultChickenManager::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FallsNum", mFallsNum);
}
