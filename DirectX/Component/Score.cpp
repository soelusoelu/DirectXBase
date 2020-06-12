#include "Score.h"
#include "ComponentManager.h"
#include "TextNumber.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"

Score::Score() :
    Component(),
    mText(nullptr),
    mScore(0),
    mHighScore(0) {
}

Score::~Score() = default;

void Score::start() {
    mText = owner()->componentManager()->getComponent<TextNumber>();
}

void Score::update() {
    if (mScore > mHighScore) {
        mHighScore = mScore;
    }
    mText->setNumber(mScore);
}

void Score::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "highScore", &mHighScore);
}

void Score::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setInt(alloc, inObj, "highScore", mHighScore);
}

void Score::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Score", mScore);
    inspect->emplace_back("HighScore", mHighScore);
}

void Score::addScore(int score) {
    mScore += score;
}

int Score::getScore() const {
    return mScore;
}
