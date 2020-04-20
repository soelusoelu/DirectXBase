#include "ScoreEvaluation.h"
#include "ChickenFry.h"
#include "FryState.h"
#include "../DebugLayer/Debug.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

ScoreEvaluation::ScoreEvaluation(std::shared_ptr<GameObject> owner) :
    Component(owner, "ScoreEvaluation"),
    mLittleBadScore(0),
    mUsuallyScore(0),
    mGoodScore(0),
    mBadScore(0) {
}

ScoreEvaluation::~ScoreEvaluation() = default;

void ScoreEvaluation::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getInt(inObj, "littleBadScore", &mLittleBadScore);
    JsonHelper::getInt(inObj, "usuallyScore", &mUsuallyScore);
    JsonHelper::getInt(inObj, "goodScore", &mGoodScore);
    JsonHelper::getInt(inObj, "badScore", &mBadScore);
}

void ScoreEvaluation::drawDebugInfo(DebugInfoList* inspect) const {
    DebugInfo info;
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

int ScoreEvaluation::evaluateScore(const ChickenFry& chicken) const {
    int score = 0;

    for (size_t i = 0; i < chicken.getNumSurface(); i++) {
        auto state = chicken.getFryState(i);
        if (state == FryState::NOT_FRIED) {
            continue;
        } else if (state == FryState::LITTLE_BAD) {
            score += mLittleBadScore;
        } else if (state == FryState::USUALLY) {
            score += mUsuallyScore;
        } else if (state == FryState::GOOD) {
            score += mGoodScore;
        } else if (state == FryState::BAD) {
            score += mBadScore;
        } else {
            Debug::logWarning(getTypeName() + ": Invalid type.");
        }
    }

    Debug::log(StringUtil::intToString(score));
    return score;
}
