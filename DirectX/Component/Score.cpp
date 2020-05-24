#include "Score.h"
#include "ComponentManager.h"
#include "Text.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Score::Score() :
    Component(),
    mText(nullptr),
    mScore(0),
    mHighScore(0) {
}

Score::~Score() = default;

void Score::start() {
    mText = owner()->componentManager()->getComponent<Text>();
}

void Score::update() {
    if (mScore > mHighScore) {
        mHighScore = mScore;
    }
    if (mText) {
        mText->setText(StringUtil::intToString(mScore));
    }
}

void Score::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getInt(inObj, "highScore", &mHighScore);
}

void Score::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setInt(alloc, inObj, "highScore", mHighScore);
}

void Score::drawDebugInfo(DebugInfoList* inspect) const {
    DebugInfo info;
    info.first = "Score";
    info.second = mScore;
    inspect->emplace_back(info);
    info.first = "HighScore";
    info.second = mHighScore;
    inspect->emplace_back(info);
}

void Score::addScore(int score) {
    mScore += score;
}

int Score::getScore() const {
    return mScore;
}
