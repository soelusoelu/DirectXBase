#include "ScoreEvaluation.h"
#include "ChickenSurface.h"
#include "FryState.h"
#include "../DebugLayer/Debug.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

ScoreEvaluation::ScoreEvaluation() :
    Component(),
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
    inspect->emplace_back("LittleBadScore", mLittleBadScore);
    inspect->emplace_back("UsuallyScore", mUsuallyScore);
    inspect->emplace_back("GoodScore", mGoodScore);
    inspect->emplace_back("BadScore", mBadScore);
}

int ScoreEvaluation::evaluateScore(const IChickenFry& chicken) const {
    int score = 0;

    auto num = static_cast<int>(ChickenSurface::NUM_SURFACE);
    for (size_t i = 0; i < num; i++) {
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
            Debug::logWarning(getComponentName() + ": Invalid type.");
        }
    }

    Debug::log(StringUtil::intToString(score));
    return score;
}
