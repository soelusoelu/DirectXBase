#include "ScoreEvaluation.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "Score.h"
#include "../DebugLayer/Debug.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

ScoreEvaluation::ScoreEvaluation(std::shared_ptr<GameObject> owner) :
    Component(owner, "ScoreEvaluation"),
    mScore(nullptr),
    mLittleBad(0.f),
    mUsually(0.f),
    mGood(0.f),
    mLittleBadScore(0),
    mUsuallyScore(0),
    mGoodScore(0),
    mBadScore(0) {
}

ScoreEvaluation::~ScoreEvaluation() = default;

void ScoreEvaluation::start() {
    auto score = owner()->getGameObjectManager()->find("Score");
    mScore = score->componentManager()->getComponent<Score>();
}

void ScoreEvaluation::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "littleBad", &mLittleBad);
    JsonHelper::getFloat(inObj, "usually", &mUsually);
    JsonHelper::getFloat(inObj, "good", &mGood);
    JsonHelper::getInt(inObj, "littleBadScore", &mLittleBadScore);
    JsonHelper::getInt(inObj, "usuallyScore", &mUsuallyScore);
    JsonHelper::getInt(inObj, "goodScore", &mGoodScore);
    JsonHelper::getInt(inObj, "badScore", &mBadScore);
}

void ScoreEvaluation::drawDebugInfo(DebugInfoList* inspect) const {
    DebugInfo info;
    info.first = "LittleBad";
    info.second = StringUtil::floatToString(mLittleBad);
    inspect->emplace_back(info);
    info.first = "Usually";
    info.second = StringUtil::floatToString(mUsually);
    inspect->emplace_back(info);
    info.first = "Good";
    info.second = StringUtil::floatToString(mGood);
    inspect->emplace_back(info);
    info.first = "LittleBadScore";
    info.second = StringUtil::floatToString(mLittleBadScore);
    inspect->emplace_back(info);
    info.first = "UsuallyScore";
    info.second = StringUtil::floatToString(mUsuallyScore);
    inspect->emplace_back(info);
    info.first = "GoodScore";
    info.second = StringUtil::floatToString(mGoodScore);
    inspect->emplace_back(info);
    info.first = "BadScore";
    info.second = StringUtil::floatToString(mBadScore);
    inspect->emplace_back(info);
}

void ScoreEvaluation::evaluateScore(const FriedChickenComponent& chicken) {
    if (!mScore) {
        return;
    }

    int score = 0;

    for (size_t i = 0; i < chicken.getNumSurface(); i++) {
        auto rate = chicken.getFryRate(i);
        if (0.f <= rate && rate < mLittleBad) {
            continue;
        } else if (mLittleBad <= rate && rate < mUsually) {
            score += mLittleBadScore;
        } else if (mUsually <= rate && rate < mGood) {
            score += mUsuallyScore;
        } else if (mGood <= rate && rate < 1.f) {
            score += mGoodScore;
        } else if (1.f <= rate) {
            score += mBadScore;
        } else {
            Debug::logWarning(getTypeName() + ": Invalid type.");
        }
    }

    if (score != 0) {
        mScore->addScore(score);
    }
}

float ScoreEvaluation::getGood() const {
    return mGood;
}
