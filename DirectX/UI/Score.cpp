#include "Score.h"
#include "../Device/DrawString.h"
#include "../Utility/LevelLoader.h"

Score::Score() :
    UI("Score"),
    mDrawPosition(Vector2::zero),
    mDrawScale(Vector2::one),
    mScore(0),
    mHighScore(0) {
}

Score::~Score() = default;

void Score::updateUI() {
    //DrawString::drawNumberRightJustified(mScore, mDrawPosition);
}

void Score::loadProperties(const rapidjson::Value& inObj) {
    UI::loadProperties(inObj);

    JsonHelper::getVector2(inObj, "position", &mDrawPosition);
    JsonHelper::getVector2(inObj, "scale", &mDrawScale);
    JsonHelper::getInt(inObj, "highScore", &mHighScore);
}

void Score::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    UI::saveProperties(alloc, inObj);

    JsonHelper::setVector2(alloc, inObj, "position", mDrawPosition);
    JsonHelper::setVector2(alloc, inObj, "scale", mDrawScale);
    JsonHelper::setInt(alloc, inObj, "highScore", mHighScore);
}
